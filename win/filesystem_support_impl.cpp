#include <zephyr/win/filesystem_support_impl.h>

#include <bitset>
#include <filesystem>
#include <fstream>

struct file_open_control_block
{
    std::string file_name;
    std::fstream fstream;
};

#ifdef __cplusplus
extern "C" {
#endif

int filesystem_stat( const char* a_path, struct filesystem_dirent_type* entry )
{
    std::error_code ec;
    std::filesystem::path path(a_path);
    bool has = std::filesystem::exists(path);
    if( !has )
    {
        return -ENOENT;
    }

    bool is_folder = std::filesystem::is_directory(path, ec);
    entry->type = is_folder ? filesystem_folder_type : filesystem_file_type;
    strncpy(entry->name, a_path, MAX_PATH_SIZE );
    size_t file_size = 0;
    if( !is_folder )
    {
        file_size = std::filesystem::file_size(path,ec);
    }
    entry->size = file_size;

    return 0;
}

int filesystem_mkdir( const char* a_path )
{
    std::error_code ec;
    std::filesystem::path path( a_path );
    bool ok = std::filesystem::create_directories( path, ec );
    return ok ? 0 : -1;
}

bool IsFlagSet( std::bitset<32>const& bits, uint32_t single_flag )
{
    std::size_t bit_pos = 0;
    uint32_t temp = single_flag;
    while( ( temp & 1 ) == 0 )
    {
        temp >>= 1;
        bit_pos++;
    }

    return bits.test( bit_pos );
}

int filesystem_open( void** zfp, const char* file_name, uint32_t flags )
{
    std::error_code ec;
    file_open_control_block* fcb = new file_open_control_block;
    fcb->file_name.assign(file_name);
    std::bitset<32> bits( flags );

    std::ios_base::openmode op_m = 0x00;
    bool need_cread_if_not = false;
    if( IsFlagSet( bits, FILESYSTEM_O_READ ) )
    {
        op_m |= std::ios_base::in;
    }
    if( IsFlagSet( bits, FILESYSTEM_O_WRITE ) )
    {
        op_m |= std::ios_base::out;
        need_cread_if_not = true;
    }
    if( IsFlagSet( bits, FILESYSTEM_O_APPEND ) )
    {
        op_m |= std::ios_base::app;
        need_cread_if_not = true;
    }
    if( IsFlagSet( bits, FILESYSTEM_O_TRUNC ) )
    {
        op_m |= std::ios_base::trunc;
        need_cread_if_not = true;
    }

    std::filesystem::path path( file_name );
    if( need_cread_if_not )
    {
        op_m |= std::ios_base::trunc;
    }

    fcb->fstream.open(file_name,op_m);
    bool is_opened = fcb->fstream.is_open();
    if( !is_opened )
    {
        delete fcb;
    }

    *zfp = fcb;
    return is_opened ? 0 : -1;
}

int filesystem_seek( void* zfp, size_t offset, int whence )
{
    file_open_control_block* fcb = reinterpret_cast< file_open_control_block* >(zfp);
    switch( whence )
    {
    case SEEK_FROM_CUR:
        fcb->fstream.seekg( offset, std::ios::cur );
        break;
    case SEEK_FROM_BEGIN:
        fcb->fstream.seekg( offset, std::ios::beg );
    break;
    case SEEK_FROM_END:
        fcb->fstream.seekg( offset, std::ios::end );
    break;
    default:
        return -1;
    }
    return 0;
}

int filesystem_read( void* zfp, char* buffer, int size )
{
    file_open_control_block* fcb = reinterpret_cast< file_open_control_block* >( zfp );
    fcb->fstream.read(buffer,size);
    int size_read = fcb->fstream.gcount();
    return size_read;
}

int filesystem_write( void* zfp, char* buffer, int size )
{
    file_open_control_block* fcb = reinterpret_cast< file_open_control_block* >( zfp );
    fcb->fstream.write(buffer,size);
    return size;
}

int filesystem_close( void* zfp )
{
    file_open_control_block* fcb = reinterpret_cast< file_open_control_block* >( zfp );
    fcb->fstream.close();
    delete fcb;
    return 0;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

