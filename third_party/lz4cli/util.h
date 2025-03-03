/* clang-format off */
/*
    util.h - utility functions
    Copyright (C) 2016-present, Przemyslaw Skibinski, Yann Collet

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef UTIL_H_MODULE
#define UTIL_H_MODULE

#if defined (__cplusplus)
extern "C" {
#endif



/*-****************************************
*  Dependencies
******************************************/

#include "third_party/lz4cli/platform.h"
#include "libc/mem/mem.h"     
#include "libc/str/str.h"     
#include "libc/stdio/stdio.h"      
#include "libc/calls/calls.h"
#include "libc/time/time.h"
#include "libc/errno.h"
#include "libc/fmt/fmt.h"
#include "libc/time/struct/utimbuf.h"
#include "libc/calls/struct/stat.h"
#include "libc/calls/struct/dirent.h"
#include "libc/calls/weirdtypes.h"



/*-**************************************************************
*  Basic Types
*****************************************************************/
#if !defined (__VMS) && (defined (__cplusplus) || (defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */) )
  typedef  uint8_t BYTE;
  typedef uint16_t U16;
  typedef  int16_t S16;
  typedef uint32_t U32;
  typedef  int32_t S32;
  typedef uint64_t U64;
  typedef  int64_t S64;
#else
  typedef unsigned char       BYTE;
  typedef unsigned short      U16;
  typedef   signed short      S16;
  typedef unsigned int        U32;
  typedef   signed int        S32;
  typedef unsigned long long  U64;
  typedef   signed long long  S64;
#endif


/* ************************************************************
* Avoid fseek()'s 2GiB barrier with MSVC, MacOS, *BSD, MinGW
***************************************************************/
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#   define UTIL_fseek _fseeki64
#elif !defined(__64BIT__) && (PLATFORM_POSIX_VERSION >= 200112L) /* No point defining Large file for 64 bit */
#  define UTIL_fseek fseeko
#elif defined(__MINGW32__) && defined(__MSVCRT__) && !defined(__STRICT_ANSI__) && !defined(__NO_MINGW_LFS)
#   define UTIL_fseek fseeko64
#else
#   define UTIL_fseek fseek
#endif


/*-****************************************
*  Sleep functions: Windows - Posix - others
******************************************/
#if defined(_WIN32)
#  include <windows.h>
#  define SET_REALTIME_PRIORITY SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)
#  define UTIL_sleep(s) Sleep(1000*s)
#  define UTIL_sleepMilli(milli) Sleep(milli)
#elif PLATFORM_POSIX_VERSION >= 0 /* Unix-like operating system */
#  include "libc/calls/calls.h"
#  if defined(PRIO_PROCESS)
#    define SET_REALTIME_PRIORITY setpriority(PRIO_PROCESS, 0, -20)
#  else
#    define SET_REALTIME_PRIORITY /* disabled */
#  endif
#  define UTIL_sleep(s) sleep(s)
#  if (defined(__linux__) && (PLATFORM_POSIX_VERSION >= 199309L)) || (PLATFORM_POSIX_VERSION >= 200112L)  /* nanosleep requires POSIX.1-2001 */
#      define UTIL_sleepMilli(milli) { struct timespec t; t.tv_sec=0; t.tv_nsec=milli*1000000ULL; nanosleep(&t, NULL); }
#  else
#      define UTIL_sleepMilli(milli) /* disabled */
#  endif
#else
#  define SET_REALTIME_PRIORITY      /* disabled */
#  define UTIL_sleep(s)          /* disabled */
#  define UTIL_sleepMilli(milli) /* disabled */
#endif


/* *************************************
*  Constants
***************************************/
#define LIST_SIZE_INCREASE   (8*1024)


/*-****************************************
*  Compiler specifics
******************************************/
#if defined(__INTEL_COMPILER)
#  pragma warning(disable : 177)    /* disable: message #177: function was declared but never referenced, useful with UTIL_STATIC */
#endif
#if defined(__GNUC__)
#  define UTIL_STATIC static __attribute__((unused))
#elif defined (__cplusplus) || (defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */)
#  define UTIL_STATIC static inline
#elif defined(_MSC_VER)
#  define UTIL_STATIC static __inline
#else
#  define UTIL_STATIC static  /* this version may generate warnings for unused static functions; disable the relevant warning */
#endif


/*-****************************************
*  Time functions
******************************************/
#if defined(_WIN32)   /* Windows */

    typedef LARGE_INTEGER UTIL_time_t;
    UTIL_STATIC UTIL_time_t UTIL_getTime(void) { UTIL_time_t x; QueryPerformanceCounter(&x); return x; }
    UTIL_STATIC U64 UTIL_getSpanTimeMicro(UTIL_time_t clockStart, UTIL_time_t clockEnd)
    {
        static LARGE_INTEGER ticksPerSecond;
        static int init = 0;
        if (!init) {
            if (!QueryPerformanceFrequency(&ticksPerSecond))
                fprintf(stderr, "ERROR: QueryPerformanceFrequency() failure\n");
            init = 1;
        }
        return 1000000ULL*(clockEnd.QuadPart - clockStart.QuadPart)/ticksPerSecond.QuadPart;
    }
    UTIL_STATIC U64 UTIL_getSpanTimeNano(UTIL_time_t clockStart, UTIL_time_t clockEnd)
    {
        static LARGE_INTEGER ticksPerSecond;
        static int init = 0;
        if (!init) {
            if (!QueryPerformanceFrequency(&ticksPerSecond))
                fprintf(stderr, "ERROR: QueryPerformanceFrequency() failure\n");
            init = 1;
        }
        return 1000000000ULL*(clockEnd.QuadPart - clockStart.QuadPart)/ticksPerSecond.QuadPart;
    }

#elif defined(__APPLE__) && defined(__MACH__)

    #include <mach/mach_time.h>
    typedef U64 UTIL_time_t;
    UTIL_STATIC UTIL_time_t UTIL_getTime(void) { return mach_absolute_time(); }
    UTIL_STATIC U64 UTIL_getSpanTimeMicro(UTIL_time_t clockStart, UTIL_time_t clockEnd)
    {
        static mach_timebase_info_data_t rate;
        static int init = 0;
        if (!init) {
            mach_timebase_info(&rate);
            init = 1;
        }
        return (((clockEnd - clockStart) * (U64)rate.numer) / ((U64)rate.denom)) / 1000ULL;
    }
    UTIL_STATIC U64 UTIL_getSpanTimeNano(UTIL_time_t clockStart, UTIL_time_t clockEnd)
    {
        static mach_timebase_info_data_t rate;
        static int init = 0;
        if (!init) {
            mach_timebase_info(&rate);
            init = 1;
        }
        return ((clockEnd - clockStart) * (U64)rate.numer) / ((U64)rate.denom);
    }

#elif (PLATFORM_POSIX_VERSION >= 200112L) && (defined __UCLIBC__ || (defined(__GLIBC__) && ((__GLIBC__ == 2 && __GLIBC_MINOR__ >= 17) || __GLIBC__ > 2) ) )

    #include "libc/time/time.h"
    typedef struct timespec UTIL_time_t;
    UTIL_STATIC UTIL_time_t UTIL_getTime(void)
    {
        UTIL_time_t now;
        if (clock_gettime(CLOCK_MONOTONIC, &now))
            fprintf(stderr, "ERROR: Failed to get time\n");   /* we could also exit() */
        return now;
    }
    UTIL_STATIC UTIL_time_t UTIL_getSpanTime(UTIL_time_t begin, UTIL_time_t end)
    {
        UTIL_time_t diff;
        if (end.tv_nsec < begin.tv_nsec) {
            diff.tv_sec = (end.tv_sec - 1) - begin.tv_sec;
            diff.tv_nsec = (end.tv_nsec + 1000000000ULL) - begin.tv_nsec;
        } else {
            diff.tv_sec = end.tv_sec - begin.tv_sec;
            diff.tv_nsec = end.tv_nsec - begin.tv_nsec;
        }
        return diff;
    }
    UTIL_STATIC U64 UTIL_getSpanTimeMicro(UTIL_time_t begin, UTIL_time_t end)
    {
        UTIL_time_t const diff = UTIL_getSpanTime(begin, end);
        U64 micro = 0;
        micro += 1000000ULL * diff.tv_sec;
        micro += diff.tv_nsec / 1000ULL;
        return micro;
    }
    UTIL_STATIC U64 UTIL_getSpanTimeNano(UTIL_time_t begin, UTIL_time_t end)
    {
        UTIL_time_t const diff = UTIL_getSpanTime(begin, end);
        U64 nano = 0;
        nano += 1000000000ULL * diff.tv_sec;
        nano += diff.tv_nsec;
        return nano;
    }

#else   /* relies on standard C (note : clock_t measurements can be wrong when using multi-threading) */

    typedef clock_t UTIL_time_t;
    UTIL_STATIC UTIL_time_t UTIL_getTime(void) { return clock(); }
    UTIL_STATIC U64 UTIL_getSpanTimeMicro(UTIL_time_t clockStart, UTIL_time_t clockEnd) { return 1000000ULL * (clockEnd - clockStart) / CLOCKS_PER_SEC; }
    UTIL_STATIC U64 UTIL_getSpanTimeNano(UTIL_time_t clockStart, UTIL_time_t clockEnd) { return 1000000000ULL * (clockEnd - clockStart) / CLOCKS_PER_SEC; }
#endif


/* returns time span in microseconds */
UTIL_STATIC U64 UTIL_clockSpanMicro(UTIL_time_t clockStart)
{
    UTIL_time_t const clockEnd = UTIL_getTime();
    return UTIL_getSpanTimeMicro(clockStart, clockEnd);
}

/* returns time span in nanoseconds */
UTIL_STATIC U64 UTIL_clockSpanNano(UTIL_time_t clockStart)
{
    UTIL_time_t const clockEnd = UTIL_getTime();
    return UTIL_getSpanTimeNano(clockStart, clockEnd);
}

UTIL_STATIC void UTIL_waitForNextTick(void)
{
    UTIL_time_t const clockStart = UTIL_getTime();
    UTIL_time_t clockEnd;
    do {
        clockEnd = UTIL_getTime();
    } while (UTIL_getSpanTimeNano(clockStart, clockEnd) == 0);
}



/*-****************************************
*  File functions
******************************************/


UTIL_STATIC int UTIL_isRegFile(const char* infilename);


UTIL_STATIC int UTIL_setFileStat(const char *filename, struct stat *statbuf)
{
    int res = 0;
    struct utimbuf timebuf;

    if (!UTIL_isRegFile(filename))
        return -1;

    timebuf.actime = time(NULL);
    timebuf.modtime = statbuf->st_mtim.tv_sec;
    res += utime(filename, &timebuf);  /* set access and modification times */

#if !defined(_WIN32)
    res += chown(filename, statbuf->st_uid, statbuf->st_gid);  /* Copy ownership */
#endif

    res += chmod(filename, statbuf->st_mode & 07777);  /* Copy file permissions */

    errno = 0;
    return -res; /* number of errors is returned */
}


UTIL_STATIC int UTIL_getFileStat(const char* infilename, struct stat *statbuf)
{
    int r;
#if defined(_MSC_VER)
    r = _stat64(infilename, statbuf);
    if (r || !(statbuf->st_mode & S_IFREG)) return 0;   /* No good... */
#else
    r = stat(infilename, statbuf);
    if (r || !S_ISREG(statbuf->st_mode)) return 0;   /* No good... */
#endif
    return 1;
}


UTIL_STATIC int UTIL_isRegFile(const char* infilename)
{
    struct stat statbuf;
    return UTIL_getFileStat(infilename, &statbuf); /* Only need to know whether it is a regular file */
}


UTIL_STATIC U32 UTIL_isDirectory(const char* infilename)
{
    int r;
    struct stat statbuf;
#if defined(_MSC_VER)
    r = _stat64(infilename, &statbuf);
    if (!r && (statbuf.st_mode & _S_IFDIR)) return 1;
#else
    r = stat(infilename, &statbuf);
    if (!r && S_ISDIR(statbuf.st_mode)) return 1;
#endif
    return 0;
}


UTIL_STATIC U64 UTIL_getFileSize(const char* infilename)
{
    int r;
#if defined(_MSC_VER)
    struct __stat64 statbuf;
    r = _stat64(infilename, &statbuf);
    if (r || !(statbuf.st_mode & S_IFREG)) return 0;   /* No good... */
#elif defined(__MINGW32__) && defined (__MSVCRT__)
    struct _stati64 statbuf;
    r = _stati64(infilename, &statbuf);
    if (r || !(statbuf.st_mode & S_IFREG)) return 0;   /* No good... */
#else
    struct stat statbuf;
    r = stat(infilename, &statbuf);
    if (r || !S_ISREG(statbuf.st_mode)) return 0;   /* No good... */
#endif
    return (U64)statbuf.st_size;
}


UTIL_STATIC U64 UTIL_getTotalFileSize(const char** fileNamesTable, unsigned nbFiles)
{
    U64 total = 0;
    unsigned n;
    for (n=0; n<nbFiles; n++)
        total += UTIL_getFileSize(fileNamesTable[n]);
    return total;
}


/*
 * A modified version of realloc().
 * If UTIL_realloc() fails the original block is freed.
*/
UTIL_STATIC void *UTIL_realloc(void *ptr, size_t size)
{
    void *newptr = realloc(ptr, size);
    if (newptr) return newptr;
    free(ptr);
    return NULL;
}


#ifdef _WIN32
#  define UTIL_HAS_CREATEFILELIST

UTIL_STATIC int UTIL_prepareFileList(const char *dirName, char** bufStart, size_t* pos, char** bufEnd)
{
    char* path;
    int dirLength, fnameLength, pathLength, nbFiles = 0;
    WIN32_FIND_DATAA cFile;
    HANDLE hFile;

    dirLength = (int)strlen(dirName);
    path = (char*) malloc(dirLength + 3);
    if (!path) return 0;

    memcpy(path, dirName, dirLength);
    path[dirLength] = '\\';
    path[dirLength+1] = '*';
    path[dirLength+2] = 0;

    hFile=FindFirstFileA(path, &cFile);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Cannot open directory '%s'\n", dirName);
        return 0;
    }
    free(path);

    do {
        fnameLength = (int)strlen(cFile.cFileName);
        path = (char*) malloc(dirLength + fnameLength + 2);
        if (!path) { FindClose(hFile); return 0; }
        memcpy(path, dirName, dirLength);
        path[dirLength] = '\\';
        memcpy(path+dirLength+1, cFile.cFileName, fnameLength);
        pathLength = dirLength+1+fnameLength;
        path[pathLength] = 0;
        if (cFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp (cFile.cFileName, "..") == 0 ||
                strcmp (cFile.cFileName, ".") == 0) continue;

            nbFiles += UTIL_prepareFileList(path, bufStart, pos, bufEnd);  /* Recursively call "UTIL_prepareFileList" with the new path. */
            if (*bufStart == NULL) { free(path); FindClose(hFile); return 0; }
        }
        else if ((cFile.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || (cFile.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) || (cFile.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)) {
            if (*bufStart + *pos + pathLength >= *bufEnd) {
                ptrdiff_t newListSize = (*bufEnd - *bufStart) + LIST_SIZE_INCREASE;
                *bufStart = (char*)UTIL_realloc(*bufStart, newListSize);
                *bufEnd = *bufStart + newListSize;
                if (*bufStart == NULL) { free(path); FindClose(hFile); return 0; }
            }
            if (*bufStart + *pos + pathLength < *bufEnd) {
                strncpy(*bufStart + *pos, path, *bufEnd - (*bufStart + *pos));
                *pos += pathLength + 1;
                nbFiles++;
            }
        }
        free(path);
    } while (FindNextFileA(hFile, &cFile));

    FindClose(hFile);
    return nbFiles;
}

#elif defined(__linux__) || (PLATFORM_POSIX_VERSION >= 200112L)  /* opendir, readdir require POSIX.1-2001 */
#  define UTIL_HAS_CREATEFILELIST
#  include "libc/calls/calls.h"
#  include "libc/str/str.h"

UTIL_STATIC int UTIL_prepareFileList(const char *dirName, char** bufStart, size_t* pos, char** bufEnd)
{
    DIR *dir;
    struct dirent *entry;
    char* path;
    int dirLength, fnameLength, pathLength, nbFiles = 0;

    if (!(dir = opendir(dirName))) {
        fprintf(stderr, "Cannot open directory '%s': %s\n", dirName, strerror(errno));
        return 0;
    }

    dirLength = (int)strlen(dirName);
    errno = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp (entry->d_name, "..") == 0 ||
            strcmp (entry->d_name, ".") == 0) continue;
        fnameLength = (int)strlen(&entry->d_name[0]);
        path = (char*) malloc(dirLength + fnameLength + 2);
        if (!path) { closedir(dir); return 0; }
        memcpy(path, dirName, dirLength);
        path[dirLength] = '/';
        memcpy(path+dirLength+1, entry->d_name, fnameLength);
        pathLength = dirLength+1+fnameLength;
        path[pathLength] = 0;

        if (UTIL_isDirectory(path)) {
            nbFiles += UTIL_prepareFileList(path, bufStart, pos, bufEnd);  /* Recursively call "UTIL_prepareFileList" with the new path. */
            if (*bufStart == NULL) { free(path); closedir(dir); return 0; }
        } else {
            if (*bufStart + *pos + pathLength >= *bufEnd) {
                ptrdiff_t newListSize = (*bufEnd - *bufStart) + LIST_SIZE_INCREASE;
                *bufStart = (char*)UTIL_realloc(*bufStart, newListSize);
                *bufEnd = *bufStart + newListSize;
                if (*bufStart == NULL) { free(path); closedir(dir); return 0; }
            }
            if (*bufStart + *pos + pathLength < *bufEnd) {
                strncpy(*bufStart + *pos, path, *bufEnd - (*bufStart + *pos));
                *pos += pathLength + 1;
                nbFiles++;
            }
        }
        free(path);
        errno = 0; /* clear errno after UTIL_isDirectory, UTIL_prepareFileList */
    }

    if (errno != 0) {
        fprintf(stderr, "readdir(%s) error: %s\n", dirName, strerror(errno));
        free(*bufStart);
        *bufStart = NULL;
    }
    closedir(dir);
    return nbFiles;
}

#else

UTIL_STATIC int UTIL_prepareFileList(const char *dirName, char** bufStart, size_t* pos, char** bufEnd)
{
    (void)bufStart; (void)bufEnd; (void)pos;
    fprintf(stderr, "Directory %s ignored (compiled without _WIN32 or _POSIX_C_SOURCE)\n", dirName);
    return 0;
}

#endif /* #ifdef _WIN32 */

/*
 * UTIL_createFileList - takes a list of files and directories (params: inputNames, inputNamesNb), scans directories,
 *                       and returns a new list of files (params: return value, allocatedBuffer, allocatedNamesNb).
 * After finishing usage of the list the structures should be freed with UTIL_freeFileList(params: return value, allocatedBuffer)
 * In case of error UTIL_createFileList returns NULL and UTIL_freeFileList should not be called.
 */
UTIL_STATIC const char** UTIL_createFileList(const char **inputNames, unsigned inputNamesNb, char** allocatedBuffer, unsigned* allocatedNamesNb)
{
    size_t pos;
    unsigned i, nbFiles;
    char* buf = (char*)malloc(LIST_SIZE_INCREASE);
    char* bufend = buf + LIST_SIZE_INCREASE;
    const char** fileTable;

    if (!buf) return NULL;

    for (i=0, pos=0, nbFiles=0; i<inputNamesNb; i++) {
        if (!UTIL_isDirectory(inputNames[i])) {
            size_t const len = strlen(inputNames[i]);
            if (buf + pos + len >= bufend) {
                ptrdiff_t newListSize = (bufend - buf) + LIST_SIZE_INCREASE;
                buf = (char*)UTIL_realloc(buf, newListSize);
                bufend = buf + newListSize;
                if (!buf) return NULL;
            }
            if (buf + pos + len < bufend) {
                strncpy(buf + pos, inputNames[i], bufend - (buf + pos));
                pos += len + 1;
                nbFiles++;
            }
        } else {
            nbFiles += UTIL_prepareFileList(inputNames[i], &buf, &pos, &bufend);
            if (buf == NULL) return NULL;
    }   }

    if (nbFiles == 0) { free(buf); return NULL; }

    fileTable = (const char**)malloc((nbFiles+1) * sizeof(const char*));
    if (!fileTable) { free(buf); return NULL; }

    for (i=0, pos=0; i<nbFiles; i++) {
        fileTable[i] = buf + pos;
        pos += strlen(fileTable[i]) + 1;
    }

    if (buf + pos > bufend) { free(buf); free((void*)fileTable); return NULL; }

    *allocatedBuffer = buf;
    *allocatedNamesNb = nbFiles;

    return fileTable;
}


UTIL_STATIC void UTIL_freeFileList(const char** filenameTable, char* allocatedBuffer)
{
    if (allocatedBuffer) free(allocatedBuffer);
    if (filenameTable) free((void*)filenameTable);
}


#if defined (__cplusplus)
}
#endif

#endif /* UTIL_H_MODULE */
