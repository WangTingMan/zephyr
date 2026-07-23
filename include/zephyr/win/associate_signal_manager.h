#pragma once

#include <zephyr/win/associate_signal.h>

#include <cstdint>
#include <vector>
#include <mutex>
#include <memory>

class associate_signal_manager
{

public:

    static associate_signal_manager& get_instance()
    {
        static associate_signal_manager instance;
        return instance;
    }

    uint64_t allocate_new_signal()
    {
        std::lock_guard locker( m_mutex_for_this );
        uint64_t id = m_next_id++;
        std::shared_ptr<associate_signal> signal;
        signal = std::make_shared<associate_signal>( id );
        m_signals.push_back( signal );
        return id;
    }

    void trigger_signal( uint64_t a_id )
    {
        std::shared_ptr<associate_signal> sig;
        std::unique_lock locker( m_mutex_for_this );
        for( auto& ele : m_signals )
        {
            if( ele->get_id() == a_id )
            {
                sig = ele;
                break;
            }
        }
        locker.unlock();

        if( sig )
        {
            sig->reset();
            sig->trigger();
        }
    }

    std::shared_ptr<associate_signal> get_associate_signal( uint64_t a_id )
    {
        std::shared_ptr<associate_signal> sig;
        std::unique_lock locker( m_mutex_for_this );
        for( auto& ele : m_signals )
        {
            if( ele->get_id() == a_id )
            {
                sig = ele;
                break;
            }
        }
        return sig;
    }

private:

    std::mutex m_mutex_for_this;
    uint64_t m_next_id = 50;
    std::vector<std::shared_ptr<associate_signal>> m_signals;
};
