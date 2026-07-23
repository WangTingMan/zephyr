#pragma once
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <semaphore>

class mutex_manager
{
public:

    static mutex_manager& get_instance();

    mutex_manager();

    uint16_t create_new_mutex();

    std::shared_ptr<std::recursive_timed_mutex> get_mutex( uint16_t a_id );

    bool release_mutex( uint16_t a_id );

private:

    std::mutex m_mutex_for_this;
    std::map<uint16_t, std::shared_ptr<std::recursive_timed_mutex>> m_mutexs;
    uint16_t m_next_id = 1;
};

class condition_variable_manager
{

public:

    static condition_variable_manager& get_instance();

    uint16_t create_new_condition_var();

    std::shared_ptr<std::condition_variable_any> get_condition_var( uint16_t a_id );

    bool release_condition_var(uint16_t id);

private:

    std::mutex m_mutex_for_this;
    std::map<uint16_t, std::shared_ptr<std::condition_variable_any>> m_condtions;
    uint16_t m_next_id = 1;
};

using semaphone_type = std::counting_semaphore<0xFFFF>;
class semaphore_manager
{
public:

    static semaphore_manager& get_instance();

    uint16_t create_new_semaphore();

    std::shared_ptr<semaphone_type> get_semaphore( uint16_t a_id );

    bool release_semaphore( uint16_t id );

private:

    std::mutex m_mutex_for_this;
    std::map<uint16_t, std::shared_ptr<semaphone_type>> m_semaphones;
    uint16_t m_next_id = 1;

};

