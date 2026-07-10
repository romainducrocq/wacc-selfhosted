#ifndef _LIB_C_LIB_H
#define _LIB_C_LIB_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// C lib

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bindings

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-library-redeclaration"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
#endif

/* inttypes.h */
extern long strtoimax(char* nptr, char** endptr, int base);
extern unsigned long strtoumax(char* nptr, char** endptr, int base);

/* stdbool.h */
#define bool int
#define false 0
#define true 1

/* stdint.h */
#ifdef __GCC_STDINT__
#include <stdint.h>
#define char_t int8_t
#define int_t int32_t
#define long_t int64_t
#define uchar_t uint8_t
#define uint_t uint32_t
#define ulong_t uint64_t
#else
#define char_t signed char
#define int_t int
#define long_t long
#define uchar_t unsigned char
#define uint_t unsigned int
#define ulong_t unsigned long
#endif

/* stdio.h */
#define NULL 0
#define FOPEN_MAX 8
struct FILE;
extern struct FILE* stdout;
extern struct FILE* stderr;
extern int fclose(struct FILE* stream);
extern int fflush(struct FILE* stream);
extern struct FILE* fopen(char* filename, char* mode);
extern unsigned long fwrite(void* ptr, unsigned long size, unsigned long nmemb, struct FILE* stream);
/* printf */
extern int fprintf(struct FILE* stream, char* format, int prec1, char* arg1, int prec2, char* arg2, int prec3,
    char* arg3, int prec4, char* arg4, int prec5, char* arg5);
extern int printf(char* format, char* arg1);
extern int snprintf(char* s, unsigned long n, char* format, char* arg1, char* arg2, char* arg3, char* arg4);
extern int sprintf(char* s, char* format, unsigned char arg1);
/* _POSIX_C_SOURCE 200809L */
extern long getline(char** lineptr, unsigned long* n, struct FILE* stream);

/* stdlib.h */
extern double strtod(char* nptr, char** endptr);
extern long strtol(char* nptr, char** endptr, int base);
extern void free(void* ptr);
extern void* malloc(unsigned long size);
extern void* realloc(void* ptr, unsigned long size);
extern void abort(void);

/* string.h */
extern void* memcpy(void* s1, void* s2, unsigned long n);
extern void* memmove(void* s1, void* s2, unsigned long n);
extern int memcmp(void* s1, void* s2, unsigned long n);
extern int strcmp(char* s1, char* s2);
extern void* memset(void* s, int c, unsigned long n);
extern unsigned long strlen(char* s);

#ifdef __clang__
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif
