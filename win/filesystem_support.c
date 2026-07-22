#include <zephyr/fs/fs.h>
#include <zephyr/win/filesystem_support_impl.h>

#include <string.h>

int fs_open( struct fs_file_t* zfp, const char* file_name, fs_mode_t flags )
{
    void* file_op = NULL;
    int st = filesystem_open(&file_op,file_name,flags);
    zfp->filep = file_op;
    return st;
}

int fs_close( struct fs_file_t* zfp )
{
    return filesystem_close(zfp->filep);
}

int fs_unlink( const char* path )
{
    remove(path);
    return 0;
}

int fs_rename( const char* from, const char* to )
{
    rename( from, to );
    return 0;
}

ssize_t fs_read( struct fs_file_t* zfp, void* ptr, size_t size )
{
    return filesystem_read(zfp->filep, ptr, size);
}

ssize_t fs_write( struct fs_file_t* zfp, const void* ptr, size_t size )
{
    return filesystem_write( zfp->filep, ptr, size );
}

int fs_seek( struct fs_file_t* zfp, off_t offset, int whence )
{
    switch( whence )
    {
    case FS_SEEK_SET:
        return filesystem_seek( zfp->filep, offset, SEEK_FROM_BEGIN );
    break;
    case FS_SEEK_CUR:
        return filesystem_seek( zfp->filep, offset, SEEK_FROM_CUR );
    break;
    case FS_SEEK_END:
        return filesystem_seek( zfp->filep, offset, SEEK_FROM_END );
    break;
        return -1;
    }

    return -1;
}

int fs_mkdir( const char* path )
{
    return filesystem_mkdir(path);
}

int fs_stat( const char* path, struct fs_dirent* entry )
{
    struct filesystem_dirent_type file_property;
    int st = filesystem_stat(path,&file_property);
    entry->size = file_property.size;
    entry->type = ( file_property.type == filesystem_file_type ) ? FS_DIR_ENTRY_FILE : FS_DIR_ENTRY_DIR;
    strncpy( entry->name, file_property.name, MAX_FILE_NAME + 1 );
    return st;
}


