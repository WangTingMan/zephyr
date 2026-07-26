#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#include <windows.h>

#include "BluetoothHomePage.h"
#include "Zhen/PageManager.h"
#include "Zhen/CommandHolder.h"
#include "Zhen/logging.h"
#include "Zhen/ExecutbleEvent.h"
#include "Zhen/KeyboardReader.h"

#if __has_include(<log/log.h>)
#include <log/log.h>
#define HAS_ANDROID_LIKE_LOG
#endif

std::ofstream s_outFile;

#define LOG_FILE "D:/bluetooth/AndroidBTInWindows.log"

#pragma comment(lib, "libZhen.lib")
#pragma comment(lib, "lib_bluetooth_app.lib")
#pragma comment(lib, "libbluetooth.lib")

#pragma comment(lib, "bluetooth.lib")
#pragma comment(lib, "zephyr_runtime.lib")
#pragma comment(lib, "utils.lib")
#pragma comment(lib, "z_crc.lib")
#pragma comment(lib, "z_settings.lib")
#pragma comment(lib, "mbedtls.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "libChromeBase.lib")
#pragma comment(lib, "ws2_32.lib")

#ifdef HAS_ANDROID_LIKE_LOG
#pragma comment(lib, "liblog.lib")
#pragma comment(lib, "libcutils.lib")
#endif

bool LogCallBackExt
    (
    const char* a_fileName,
    int a_lineNumber,
    Zhen::LogLevel a_level,
    std::string const& a_tag,
    std::string const& a_log
    );

int main()
{
    KeyboardReader reader;
    reader.start();

    s_outFile.open( LOG_FILE, std::ios::out | std::ios::trunc );
    Zhen::SetLogCallBack( std::bind( &LogCallBackExt, std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3, std::placeholders::_4, std::placeholders::_5 ) );

    std::shared_ptr<BasePage> page = std::make_shared<BluetoothHomePage>();
    PageManager::GetInstance().PushPage( std::move( page ) );

    PageManager::GetInstance().run();
}

bool LogCallBackExt
    (
    const char* a_fileName,
    int a_lineNumber,
    Zhen::LogLevel a_level,
    std::string const& a_tag,
    std::string const& a_log
    )
{
    const char* file = a_fileName;
    const char* split = a_fileName;
    while( file && *( file++ ) != '\0' )
    {
        if( '/' == *file ||
            '\\' == *file )
        {
            split = file + 1;
        }
    }
    if( !split )
    {
        split = "Unkown";
    }

    const char* type_s = "?";
    switch( a_level )
    {
    case Zhen::LogLevel::Info:
        type_s = "I";
    break;
    case Zhen::LogLevel::Debug:
        type_s = "D";
    break;
    case Zhen::LogLevel::Warning:
        type_s = "W";
    break;
    case Zhen::LogLevel::Error:
        type_s = "E";
    break;
    case Zhen::LogLevel::Fatal:
        type_s = "F";
    break;
    default:
    break;
    }

    SYSTEMTIME sys;
    GetLocalTime( &sys );
    std::string timeStr;
    char buffer[100];
    sprintf_s( buffer, "[%02d-%02d %02d:%02d:%02d.%03d] ", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute,
        sys.wSecond, sys.wMilliseconds );

    std::stringstream ss;
    ss << buffer << "[" << type_s << "][" << split << ":" << a_lineNumber << "] " << a_log << std::endl;
    s_outFile << ss.str();
    s_outFile.flush();
    if( s_outFile.is_open() )
    {
        ss.clear();
    }
    return false;
}
