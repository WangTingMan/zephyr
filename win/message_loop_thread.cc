/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr/win/message_loop_thread.h>

#include <base/functional/callback.h>
#include <base/location.h>
#include <base/time/time.h>

#include <chrono>
#include <format>
#include <future>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include <zephyr/win/kernel_support_impl.h>

namespace bluetooth {
namespace common {

static constexpr int kRealTimeFifoSchedulingPriority = 1;

static base::TimeDelta timeDeltaFromMicroseconds(std::chrono::microseconds t) {
#if BASE_VER < 931007
  return base::TimeDelta::FromMicroseconds(t.count());
#else
  return base::Microseconds(t.count());
#endif
}

MessageLoopThread::MessageLoopThread(const std::string& thread_name)
    : thread_name_(thread_name),
      message_loop_(nullptr),
      run_loop_(nullptr),
      thread_(nullptr),
      thread_id_(-1),
      weak_ptr_factory_(this),
      shutting_down_(false) {
  // Thread and Handler will be initiated in StartUp().
}

MessageLoopThread::~MessageLoopThread() { ShutDown(); }

void MessageLoopThread::StartUp() {
  std::promise<void> start_up_promise;
  std::future<void> start_up_future = start_up_promise.get_future();
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    if (thread_ != nullptr) {
      return;
    }
    thread_ = new std::thread(&MessageLoopThread::RunThread, this, std::move(start_up_promise));
  }
  start_up_future.wait();
}

bool MessageLoopThread::DoInThread( std::function<void()> a_task )
{
    return DoInThreadDelayed( std::move( a_task ), std::chrono::microseconds( 0 ) );
}

bool MessageLoopThread::DoInThread(base::OnceClosure task) {
  return DoInThreadDelayed(std::move(task), std::chrono::microseconds(0));
}

bool MessageLoopThread::DoInThreadDelayed(base::OnceClosure task, std::chrono::microseconds delay) {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  if (message_loop_ == nullptr) {
    /*log::error("message loop is null for thread {}", *this);*/
    return false;
  }
  if (!message_loop_->task_runner()->PostDelayedTask(FROM_HERE, std::move(task),
                                                     timeDeltaFromMicroseconds(delay))) {
    /*log::error("failed to post task to message loop for thread {}", *this);*/
    return false;
  }
  return true;
}

bool MessageLoopThread::DoInThreadDelayed( std::function<void()> task, std::chrono::microseconds delay )
{
    return DoInThreadDelayed( base::BindOnce( []( std::function<void()> a_tsk )
        {
            a_tsk();
        }, task ), delay );
}

void MessageLoopThread::ShutDown() {
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    if (thread_ == nullptr) {
      /*log::info("thread {} is already stopped", *this);*/
      return;
    }
    if (message_loop_ == nullptr) {
      /*log::info("message_loop_ is null. Already stopping");*/
      return;
    }
    if (shutting_down_) {
      /*log::info("waiting for thread to join");*/
      return;
    }
    shutting_down_ = true;
//     log::assert_that(thread_id_ != base::PlatformThread::CurrentId(),
//                      "should not be called on the thread itself. Otherwise, deadlock may happen.");
    run_loop_->QuitWhenIdle();
  }
  thread_->join();
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
    delete thread_;
    thread_ = nullptr;
    shutting_down_ = false;
  }
}

base::PlatformThreadId MessageLoopThread::GetThreadId() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return thread_id_;
}

bool MessageLoopThread::IsRunningOnSameThread() const {
  return thread_id_ == base::PlatformThread::CurrentId();
}

std::string MessageLoopThread::GetName() const { return thread_name_; }

std::string MessageLoopThread::ToString() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
#if defined(TARGET_FLOSS) && BASE_VER >= 1419016
  return std::format("{}({})", thread_name_, thread_id_.raw());
#else
  return std::format("{}({})", thread_name_, thread_id_);
#endif  // defined(TARGET_FLOSS) && BASE_VER >= 1419016
}

bool MessageLoopThread::IsRunning() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
#if defined(TARGET_FLOSS) && BASE_VER >= 1419016
  return thread_id_.raw() != -1;
#else
  return thread_id_ != -1;
#endif  // defined(TARGET_FLOSS) && BASE_VER >= 1419016
}

// Non API method, should not be protected by API mutex
void MessageLoopThread::RunThread(MessageLoopThread* thread, std::promise<void> start_up_promise) {
  thread->Run(std::move(start_up_promise));
}

// This is only for use in tests.
base::MessageLoop* MessageLoopThread::message_loop() const {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return message_loop_;
}

bool MessageLoopThread::EnableRealTimeScheduling() {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);

  if (!IsRunning()) {
    /*log::error("thread {} is not running", *this);*/
    return false;
  }
  return true;
}

// Note: Crash if called when flag replace_message_loop_thread_with_gd_handler is enabled.
base::WeakPtr<MessageLoopThread> MessageLoopThread::GetWeakPtr() {
  std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
  return weak_ptr_factory_.GetWeakPtr();
}

void MessageLoopThread::Run(std::promise<void> start_up_promise) {
  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);

    /*log::info("message loop starting for thread {}", thread_name_);*/
    base::PlatformThread::SetName(thread_name_);
    message_loop_ = new base::MessageLoop();
    run_loop_ = new base::RunLoop();
    thread_id_ = base::PlatformThread::CurrentId();
    start_up_promise.set_value();
  }

  // Blocking until ShutDown() is called
  run_loop_->Run();

  {
    std::lock_guard<std::recursive_mutex> api_lock(api_mutex_);
#if defined(TARGET_FLOSS) && BASE_VER >= 1419016
    thread_id_ = base::PlatformThreadId(-1);
#else
    thread_id_ = -1;
#endif  // defined(TARGET_FLOSS) && BASE_VER >= 1419016
    delete message_loop_;
    message_loop_ = nullptr;
    delete run_loop_;
    run_loop_ = nullptr;
  }
}

void MessageLoopThread::Post(base::OnceClosure closure) { DoInThread(std::move(closure)); }

}  // namespace common
}  // namespace bluetooth

message_loop_thread_manager& message_loop_thread_manager::get_instance()
{
    static message_loop_thread_manager instance;
    return instance;
}

uint64_t message_loop_thread_manager::create_new_message_loop( std::string a_name )
{
    std::shared_ptr<bluetooth::common::MessageLoopThread> thread;
    thread = std::make_shared<bluetooth::common::MessageLoopThread>( a_name );
    thread->StartUp();
    uint64_t id;
    id = thread->GetThreadId();

    std::lock_guard locker(m_mtex);
    m_threads[id] = thread;
    return id;
}

std::shared_ptr<bluetooth::common::MessageLoopThread> message_loop_thread_manager::get_message_loop( uint64_t a_id )
{
    std::lock_guard locker( m_mtex );
    for( auto& ele : m_threads )
    {
        if( ele.first == a_id )
        {
            return ele.second;
        }
    }
    return nullptr;
}

void message_loop_thread_manager::quit_message_loop( uint64_t a_id )
{
    std::shared_ptr<bluetooth::common::MessageLoopThread> thread;
    std::unique_lock locker( m_mtex );
    for( auto it = m_threads.begin(); it != m_threads.end(); ++it )
    {
        if( it->first == a_id )
        {
            thread = it->second;
            m_threads.erase(it);
            break;
        }
    }

    locker.unlock();

    if( thread )
    {
        thread->ShutDown();
    }
}

std::shared_ptr<k_thread> message_loop_thread_manager::get_thread_cb( uint64_t a_tid )
{
    std::unique_lock locker( m_mtex );
    for( auto& th_cb : m_k_thread_cbs )
    {
        uint64_t id = get_underlying_id_for_thread(th_cb.get());
        if( id == a_tid )
        {
            return th_cb;
        }
    }

    std::shared_ptr<k_thread> _thread;
    _thread.reset( allocate_one_k_thread(), free_resource );
    set_underlying_id_for_thread(_thread.get(), base::PlatformThread::CurrentId() );

    m_k_thread_cbs.push_back(_thread);
    return _thread;
}

extern "C"
{

struct k_thread* create_k_thread( const char* a_name, uint64_t* a_id )
{
    struct k_thread* cb = NULL;
    std::shared_ptr<k_thread> _thread;
    _thread.reset( allocate_one_k_thread(), free_resource );
    std::string name;
    if( a_name )
    {
        name.assign( a_name );
    }

    uint64_t id = 0;
    id = message_loop_thread_manager::get_instance().create_new_message_loop( name );
    *a_id = id;
    cb = _thread.get();

    message_loop_thread_manager::get_instance().keep_thread_cb(_thread);
    return cb;
}

k_thread* get_current_thread_info()
{
    uint64_t id = base::PlatformThread::CurrentId();
    auto cb = message_loop_thread_manager::get_instance().get_thread_cb(id);
    return cb.get();
}

int post_task_to_thread
    (
    uint64_t a_thread_id,
    function_type a_task,
    void* a_parameters,
    uint32_t a_delay_time_in_milliseconds
    )
{
    auto thread = message_loop_thread_manager::get_instance().get_message_loop(a_thread_id);
    if( !thread )
    {
        // TODO ? no such thread!
        return -90;
    }

    std::function<void()> fun = std::bind(a_task, a_parameters);
    thread->DoInThreadDelayed(fun, std::chrono::milliseconds(a_delay_time_in_milliseconds));
    return 0;
}

}

