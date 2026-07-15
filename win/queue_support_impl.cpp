#include <zephyr/win/queue_support_impl.h>

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

class queue_support_entity
{

public:

    uint16_t get_id()const
    {
        std::lock_guard locker( m_this_mutex );
        return m_id;
    }

    void set_id( uint16_t id )
    {
        std::lock_guard locker( m_this_mutex );
        m_id = id;
    }

    void* peek_front()
    {
        std::lock_guard locker( m_this_mutex );
        if( m_detail_queue.empty() )
        {
            return nullptr;
        }

        return m_detail_queue.front();
    }

    void append( void* data )
    {
        std::lock_guard locker( m_this_mutex );
        m_detail_queue.push_back( data );
        m_condition.notify_all();
    }

    void prepend( void* data )
    {
        std::lock_guard locker( m_this_mutex );
        m_detail_queue.push_front(data);
        m_condition.notify_all();
    }

    bool empty()
    {
        std::lock_guard locker(m_this_mutex);
        return m_detail_queue.empty();
    }

    void init()
    {
        m_detail_queue.clear();
    }

    void* get_front( uint32_t timeout )
    {
        void* data = nullptr;
        std::unique_lock locker(m_this_mutex);
        if( !m_detail_queue.empty() )
        {
            data = m_detail_queue.front();
            m_detail_queue.pop_front();
            return data;
        }

        m_condition.wait_for( locker, std::chrono::milliseconds( timeout ), [this]()
            {
                return !m_detail_queue.empty();
            });

        if( !m_detail_queue.empty() )
        {
            data = m_detail_queue.front();
            m_detail_queue.pop_front();
            return data;
        }
        return data;
    }

private:

    mutable std::mutex m_this_mutex;
    std::condition_variable m_condition;
    uint16_t m_id = 0x00;
    std::deque<void*> m_detail_queue;
};

class queue_support_manager
{

public:

    static queue_support_manager& get_instance()
    {
        static queue_support_manager instance;
        return instance;
    }

    std::shared_ptr<queue_support_entity> find_queue_by_id( uint16_t id )
    {
        std::lock_guard locker(m_this_mutex);
        for( auto& ele : m_managed_queues )
        {
            if( ele->get_id() == id )
            {
                return ele;
            }
        }
        return nullptr;
    }

    std::shared_ptr<queue_support_entity> create_new_queue()
    {
        std::lock_guard locker( m_this_mutex );
        uint16_t id = m_next_id++;

        auto queue = std::make_shared<queue_support_entity>();
        queue->set_id(id);
        m_managed_queues.push_back(queue);

        return queue;
    }

private:

    std::mutex m_this_mutex;
    uint16_t m_next_id = 1;
    std::vector<std::shared_ptr<queue_support_entity>> m_managed_queues;
};

#ifdef __cplusplus
extern "C" {
#endif

void queue_prepend( uint16_t id, void* data )
{
    auto queue = queue_support_manager::get_instance().find_queue_by_id(id);
    if( queue )
    {
        queue->prepend(data);
    }
    else
    {
        abort();
    }
}

int queue_is_empty( uint16_t id )
{
    int status = 0;
    auto queue = queue_support_manager::get_instance().find_queue_by_id( id );
    if( queue )
    {
        status = queue->empty() ? 1 : 0;
    }
    else
    {
        abort();
    }
    return status;
}

void* queue_get( uint16_t id, uint32_t timeout )
{
    auto queue = queue_support_manager::get_instance().find_queue_by_id( id );
    if( queue )
    {
        return queue->get_front(timeout);
    }
    else
    {
        abort();
    }
    return nullptr;
}

uint16_t queue_init( uint16_t id )
{
    auto queue = queue_support_manager::get_instance().find_queue_by_id( id );
    if( queue )
    {
        queue->init();
    }
    else
    {
        queue = queue_support_manager::get_instance().create_new_queue();
        id = queue->get_id();
    }
    return id;
}

void queue_append( uint16_t id, void* data )
{
    auto queue = queue_support_manager::get_instance().find_queue_by_id( id );
    queue->append(data);
}

void* queue_peek_head( uint16_t id )
{
    auto queue = queue_support_manager::get_instance().find_queue_by_id( id );
    return queue->peek_front();
}

#ifdef __cplusplus
} /* extern "C" */
#endif
