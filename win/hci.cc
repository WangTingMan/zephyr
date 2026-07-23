#include <stdint.h>
#include <future>
#include <filesystem>

#include <zephyr/win/kernel_support_impl.h>

extern "C"
{
#if (PLATFORM_TYPE == ACTIONS_PLATFORM)
#include <drivers/bluetooth/bt_drv.h>
#endif

}

#include "windows/windows_usb_transport.h"
#include "hardware/chipset_initializer.h"
#include <hardware/log.h>

windows_usb_transport s_windows_usb_transport;;
std::shared_ptr<chipset_initializer> s_chipset_initializer;
std::promise<void> hci_initialization_;
simple_hci_snoop_logger s_snoop_logger;

#if (PLATFORM_TYPE == ACTIONS_PLATFORM)
btdrv_hci_cb_t* hci_cb = nullptr;
#elif (PLATFORM_TYPE == ZEPHYR_ORIGINAL)
extern "C" void hci_data_receive_zephyr_original( uint8_t a_type, uint8_t* a_data, uint16_t a_size);
#endif

#define LOGGER(...)

std::string GetCurrentDir();
std::vector<std::string> prepare_intel_fw_path( std::string a_fw_name );
extern "C" void hci_initialize();
void capture_hci_during_initializing
    (
    uint8_t a_type,
    uint8_t* a_data,
    uint16_t a_size
    );
void initialization_complete();
void hci_data_receive( uint8_t a_type, uint8_t* a_data, uint16_t a_size );


extern "C"
{

int btdrv_send( uint8_t type, uint8_t* p_data, uint16_t len )
{
    s_snoop_logger.write(type,p_data, len, false);
    int status = 0;
    switch( type )
    {
    case 0x01:
        status = s_windows_usb_transport.send_data_to_controller( 0x01, p_data, len );
        break;
    case 0x02:
        status = s_windows_usb_transport.send_data_to_controller( 0x02, p_data, len );
    break;
    case 0x03:
        status = s_windows_usb_transport.send_data_to_controller( 0x03, p_data, len );
    break;
    }
    return 0;
}

#if (PLATFORM_TYPE == ACTIONS_PLATFORM)
int btdrv_init( btdrv_hci_cb_t* cb )
{
    hci_cb = cb;
    auto future_ = hci_initialization_.get_future();
    hci_initialize();
    future_.wait();
    return 0;
}
#endif

int btdrv_exit( void )
{
    return 0;
}

}

std::vector<std::string> prepare_intel_fw_path( std::string a_fw_name )
{
    std::vector<std::string> paths;

    std::string cur_dir = GetCurrentDir();
    if( cur_dir.back() != '/' &&
        cur_dir.back() != '\\' )
    {
        cur_dir.push_back( '/' );
    }

    cur_dir.append( "intel_fw/" ).append( a_fw_name );

    paths.push_back( cur_dir );

    return paths;
}

extern "C" void hci_initialize()
{
    int status_ret = -1;

    std::string cur_dir = GetCurrentDir();
    if( cur_dir.back() != '/' &&
        cur_dir.back() != '\\' )
    {
        cur_dir.push_back( '/' );
    }
    cur_dir.append( "hci_snoop.cfa" );

    std::string hci_log_file = cur_dir;
    s_snoop_logger.open_next_file( hci_log_file );
    set_transport_logger( []( __log_message a_log )
        {
        } );

    bool status = s_windows_usb_transport.open_bluetooth_usb();
    if( !status )
    {
        LOGGER( "cannot open USB bluetooth device" );
        return;
    }

    s_chipset_initializer = create_chipset_initializer( s_windows_usb_transport.get_pid(), s_windows_usb_transport.get_vid() );
    if( !s_chipset_initializer )
    {
        LOGGER( "this is a new bluetooth usb device, please notify me." );
        return;
    }

    s_chipset_initializer->set_fw_file_preparer( std::bind( &prepare_intel_fw_path,
        std::placeholders::_1 ) );

    s_windows_usb_transport.set_controller_data_handler( std::bind(
        &chipset_initializer::hci_packet_bringup_handler, s_chipset_initializer,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );

    chipset_initializer::hci_sender sender;
    chipset_initializer::initialize_completed_callback initialize_callback;
    sender = std::bind( &windows_usb_transport::send_data_to_controller,
        &s_windows_usb_transport, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
    initialize_callback = []( bool a_result )
        {
            controller_data_handler_type handler;
            handler = std::bind(&hci_data_receive, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            s_windows_usb_transport.set_controller_data_handler( handler );
            initialization_complete();
        };
    s_chipset_initializer->set_sender( sender );
    s_chipset_initializer->set_hci_recorder( std::bind( &capture_hci_during_initializing,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
    s_chipset_initializer->set_initialization_callback( initialize_callback );

    int init_status = s_chipset_initializer->initialize();
    return;
}

void initialization_complete()
{
    hci_initialization_.set_value();
}

void capture_hci_during_initializing
    (
    uint8_t a_type,
    uint8_t* a_data,
    uint16_t a_size
    )
{
    s_snoop_logger.write( a_type, a_data, a_size, true );
    return;
}

void hci_data_receive( uint8_t a_type, uint8_t* a_data, uint16_t a_size )
{
    s_snoop_logger.write( a_type, a_data, a_size, true );
    uint8_t* buffer = nullptr;
    if( a_size < 1 )
    {
        return;
    }

    uint8_t event_code = 0x00;
    if( a_type == 0x04 )
    {
        event_code = a_data[0];
    }

#if (PLATFORM_TYPE == ACTIONS_PLATFORM)
    buffer = hci_cb->get_buf(a_type, event_code, a_size);
    if( buffer )
    {
        memcpy(buffer, a_data, a_size);
        hci_cb->recv(a_size);
    }
#elif (PLATFORM_TYPE == ZEPHYR_ORIGINAL)
    hci_data_receive_zephyr_original( a_type, a_data, a_size );
#endif
}

std::string GetCurrentDir()
{
    wchar_t exeFullPath[MAX_PATH]; // Full path
    std::wstring strPath;

    GetModuleFileName( NULL, exeFullPath, MAX_PATH );
    strPath = ( std::wstring )exeFullPath;    // Get full path of the file
    int pos = strPath.find_last_of( '\\', strPath.length() );
    std::filesystem::path pth( strPath.substr( 0, pos ) );
    std::string ret = pth.generic_string();
    ret.push_back( '\\' );
    return ret;  // Return the directory without the file name
}
