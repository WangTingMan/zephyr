#include <zephyr/fs/fs.h>

int fs_open( struct fs_file_t* zfp, const char* file_name, fs_mode_t flags )
{
    // TODO

    return 0;
}

int fs_close( struct fs_file_t* zfp )
{
    // TODO
    return 0;
}

int fs_unlink( const char* path )
{
    // TODO
    return 0;
}

int fs_rename( const char* from, const char* to )
{
    // TODO
    return 0;
}

ssize_t fs_read( struct fs_file_t* zfp, void* ptr, size_t size )
{
    // TODO
    return 0;
}

ssize_t fs_write( struct fs_file_t* zfp, const void* ptr, size_t size )
{
    // TODO
    return 0;
}

int fs_seek( struct fs_file_t* zfp, off_t offset, int whence )
{
    // TODO
    return 0;
}

int fs_mkdir( const char* path )
{
    // TODO
    return 0;
}

int fs_stat( const char* path, struct fs_dirent* entry )
{
    // TODO
    return 0;
}


