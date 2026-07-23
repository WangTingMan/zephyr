#pragma once
#include <stdint.h>

enum filesystem_entry_type
{
    /** Identifier for file entry */
    filesystem_file_type = 0,
    /** Identifier for directory entry */
    filesystem_folder_type
};

/** Open for read flag */
#define FILESYSTEM_O_READ       0x01
/** Open for write flag */
#define FILESYSTEM_O_WRITE      0x02
/** Open for read-write flag combination */
#define FILESYSTEM_O_RDWR       (FILESYSTEM_O_READ | FILESYSTEM_O_WRITE)
/** Bitmask for read and write flags */
#define FILESYSTEM_O_MODE_MASK  0x03

/** Create file if it does not exist */
#define FILESYSTEM_O_CREATE     0x10
/** Open/create file for append */
#define FILESYSTEM_O_APPEND     0x20
/** Truncate the file while opening */
#define FILESYSTEM_O_TRUNC      0x40
/** Bitmask for open/create flags */
#define FILESYSTEM_O_FLAGS_MASK 0x70

#define SEEK_FROM_CUR 0
#define SEEK_FROM_BEGIN 1
#define SEEK_FROM_END 2

#define MAX_PATH_SIZE 256
struct filesystem_dirent_type
{
    /**
     * File/directory type (FS_DIR_ENTRY_FILE or FS_DIR_ENTRY_DIR)
     */
    enum filesystem_entry_type type;
    /** Name of file or directory */
    char name[MAX_PATH_SIZE];
    /** Size of file (0 if directory). */
    size_t size;
};

#ifdef __cplusplus
extern "C" {
#endif

int filesystem_stat( const char* path, struct filesystem_dirent_type* entry );

int filesystem_mkdir( const char* path );

int filesystem_open( void** zfp, const char* file_name, uint32_t flags );

int filesystem_seek( void* zfp, size_t offset, int whence );

int filesystem_read( void* zfp, char* buffer, int size );

int filesystem_write( void* zfp, char* buffer, int size );

int filesystem_close( void* zfp );

#ifdef __cplusplus
} /* extern "C" */
#endif
