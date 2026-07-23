#include <zephyr/win/mutex_support.h>

extern "C" uint16_t s_irq_mutex_id = 0x00;

mutex_manager& mutex_manager::get_instance()
{
    static mutex_manager instance;
    return instance;
}

mutex_manager::mutex_manager()
{
    s_irq_mutex_id = create_new_mutex();
}

uint16_t mutex_manager::create_new_mutex()
{
    std::lock_guard locker( m_mutex_for_this );
    std::shared_ptr<std::recursive_timed_mutex> mutex = std::make_shared<std::recursive_timed_mutex>();
    uint16_t id = m_next_id++;
    m_mutexs[id] = mutex;
    return id;
}

std::shared_ptr<std::recursive_timed_mutex> mutex_manager::get_mutex( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_mutexs.find( a_id );
    if( it == m_mutexs.end() )
    {
        return nullptr;
    }

    return it->second;
}

bool mutex_manager::release_mutex( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_mutexs.find( a_id );
    if( it == m_mutexs.end() )
    {
        return false;
    }

    m_mutexs.erase( a_id );
    return true;
}

condition_variable_manager& condition_variable_manager::get_instance()
{
    static condition_variable_manager instance;
    return instance;
}

uint16_t condition_variable_manager::create_new_condition_var()
{
    std::lock_guard locker( m_mutex_for_this );
    std::shared_ptr<std::condition_variable_any> cond = std::make_shared<std::condition_variable_any>();
    uint16_t id = m_next_id++;
    m_condtions[id] = cond;
    return id;
}

std::shared_ptr<std::condition_variable_any> condition_variable_manager::get_condition_var( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_condtions.find( a_id );
    if( it == m_condtions.end() )
    {
        return nullptr;
    }

    return it->second;
}

bool condition_variable_manager::release_condition_var( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_condtions.find( a_id );
    if( it == m_condtions.end() )
    {
        return false;
    }

    m_condtions.erase( a_id );
    return true;
}

semaphore_manager& semaphore_manager::get_instance()
{
    static semaphore_manager instance;
    return instance;
}

uint16_t semaphore_manager::create_new_semaphore()
{
    std::lock_guard locker( m_mutex_for_this );
    std::shared_ptr<semaphone_type> sema = std::make_shared<semaphone_type>(0);
    uint16_t id = m_next_id++;
    m_semaphones[id] = sema;
    return id;
}

std::shared_ptr<semaphone_type> semaphore_manager::get_semaphore( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_semaphones.find( a_id );
    if( it == m_semaphones.end() )
    {
        return nullptr;
    }

    return it->second;
}

bool semaphore_manager::release_semaphore( uint16_t a_id )
{
    std::lock_guard locker( m_mutex_for_this );
    auto it = m_semaphones.find( a_id );
    if( it == m_semaphones.end() )
    {
        return false;
    }

    m_semaphones.erase( a_id );
    return true;
}

extern "C"
{

uint16_t allocate_one_mutex()
{
    return mutex_manager::get_instance().create_new_mutex();
}

int require_mutex( uint16_t a_id, uint32_t timeout )
{
    int status = 0;
    auto mutex = mutex_manager::get_instance().get_mutex(a_id);
    if( mutex )
    {
        if( timeout > 0xFFFFFF )
        {
            mutex->lock();
            status = 0;
            return status;
        }
        else if( timeout == 0x00 )
        {
            bool locked = mutex->try_lock();
            status = locked ? 1 : 0;
            return status;
        }
        else
        {
            bool locked = false;
            locked = mutex->try_lock_for(std::chrono::milliseconds(timeout));
            status = locked ? 1 : 0;
            return status;
        }
    }
    return 0;
}

int require_mutex_timeout( uint16_t a_id, uint32_t a_timeout )
{
    auto mutex = mutex_manager::get_instance().get_mutex( a_id );
    if( mutex )
    {
        bool status = mutex->try_lock_for(std::chrono::milliseconds(a_timeout));
        return status ? 1 : 0;
    }
    return 0;
}

int release_mutex( uint16_t a_id )
{
    auto mutex = mutex_manager::get_instance().get_mutex( a_id );
    if( mutex )
    {
#pragma warning(push)
#pragma warning(disable: 26110)
        mutex->unlock();
#pragma warning(pop)
        return 1;
    }
    return 0;
}

int free_mutex( uint16_t a_id )
{
    mutex_manager::get_instance().release_mutex( a_id );
    return 1;
}

int free_condition_variable( uint16_t a_id )
{
    bool status = condition_variable_manager::get_instance().release_condition_var(a_id);
    return status ? 0 : -90;
}

uint16_t allocate_one_condition_variable()
{
    uint16_t id = condition_variable_manager::get_instance().create_new_condition_var();
    return id;
}

int condition_variable_wait_for( uint16_t a_cond_id, uint16_t a_mutex_id, uint32_t a_timeout )
{
    auto mutex = mutex_manager::get_instance().get_mutex(a_mutex_id);
    auto condition = condition_variable_manager::get_instance().get_condition_var(a_cond_id);
    if( !mutex || !condition )
    {
        return -90;
    }

    std::unique_lock locker(*mutex);
    auto status = condition->wait_for(locker, std::chrono::milliseconds(a_timeout));
    if( status == std::cv_status::timeout )
    {
        return -90;
    }

    locker.unlock();
    mutex->lock();
    return 0;
}

int condition_var_notify_all( uint16_t a_id )
{
    auto condition = condition_variable_manager::get_instance().get_condition_var( a_id );
    if( condition )
    {
        condition->notify_all();
        return 0;
    }
    return -90;
}

uint16_t allocate_one_semaphone()
{
    return semaphore_manager::get_instance().create_new_semaphore();
}

int semaphone_require( uint16_t a_id, uint32_t a_timeout )
{
    auto seama = semaphore_manager::get_instance().get_semaphore( a_id );
    if( seama )
    {
        bool status = seama->try_acquire_for(std::chrono::milliseconds(a_timeout));
        return status ? 0 : -90;
    }
    return -90;
}

int seamaphone_free( uint16_t a_id )
{
    auto seama = semaphore_manager::get_instance().get_semaphore( a_id );
    if( seama )
    {
        seama->release();
        return 0;
    }
    return -90;
}

int seamaphone_release( uint16_t a_id, uint32_t count )
{
    auto seama = semaphore_manager::get_instance().get_semaphore( a_id );
    if( seama )
    {
        for( uint32_t i = 0; i < count; ++i )
        {
            seama->release();
        }
        return 0;
    }
    return -90;
}

int semaphone_clear_count(uint16_t a_id)
{
    auto seama = semaphore_manager::get_instance().get_semaphore( a_id );
    if( seama )
    {
        // TODO, STD does not support clear the count.. we need use conditon to impl it.
        seama->release(0xffff);
        return 0;
    }
    return -90;
}

int semaphone_get_count( uint16_t a_id )
{
    auto seama = semaphore_manager::get_instance().get_semaphore( a_id );
    if( seama )
    {
        // TODO, STD does not support clear the count.. we need use conditon to impl it.
        seama->release();
        return 0;
    }
    return 0;
}

}
