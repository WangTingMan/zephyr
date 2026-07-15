#include <zephyr/net_buf.h>
#include <zephyr/win/queue_support_impl.h>

#define NET_POOL_SIZE 100
struct net_buf_pool* _net_buf_pool_list[NET_POOL_SIZE] = {0x00};
uint32_t s_next_id = 0;

void register_net_buf_pool_instance( struct net_buf_pool* instance )
{
    uint32_t id = s_next_id;
    s_next_id++;
    instance->this_pool_id = id;
    _net_buf_pool_list[id] = instance;
    instance->free._queue.underlying_id = queue_init(0);
}

struct net_buf_pool* retrieve_net_buf_pool( uint32_t id )
{
    struct net_buf_pool* ret = NULL;
    for( int i = 0; i < NET_POOL_SIZE; ++i )
    {
        struct net_buf_pool* it = _net_buf_pool_list[i];
        if( !it )
        {
            break;
        }

        if( it->this_pool_id == id )
        {
            ret = it;
            break;
        }
    }
    return ret;
}

void initialize_net_pool()
{
}

