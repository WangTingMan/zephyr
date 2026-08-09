#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef _MSC_VER
#define PRIVILEGED_FUNCTION
#endif

#ifndef PRIVILEGED_FUNCTION
#define PRIVILEGED_FUNCTION
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Used to pass information about the heap out of vPortGetHeapStats(). */
typedef struct xHeapStats
{
    size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t xSizeOfLargestFreeBlockInBytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xNumberOfFreeBlocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t xNumberOfSuccessfulAllocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t xNumberOfSuccessfulFrees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
void vPortGetHeapStats( HeapStats_t* pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */
void* pvPortMalloc( size_t xWantedSize ) PRIVILEGED_FUNCTION;
void* pvPortCalloc( size_t xNum,
    size_t xSize ) PRIVILEGED_FUNCTION;
void vPortFree( void* pv ) PRIVILEGED_FUNCTION;
void vPortInitialiseBlocks( void ) PRIVILEGED_FUNCTION;
size_t xPortGetFreeHeapSize( void ) PRIVILEGED_FUNCTION;
size_t xPortGetMinimumEverFreeHeapSize( void ) PRIVILEGED_FUNCTION;
void xPortResetHeapMinimumEverFreeHeapSize( void ) PRIVILEGED_FUNCTION;

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
void vPortHeapResetState( void ) PRIVILEGED_FUNCTION;

#ifdef __cplusplus
    }
#endif

