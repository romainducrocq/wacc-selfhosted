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

/* stdbool.h */
#define bool int
#define false 0
#define true 1

/* stdio.h */
#define NULL ((void*)0)
#define FOPEN_MAX 16
struct FILE;
extern int fclose(struct FILE* stream);
extern struct FILE* fopen(char* filename, char* mode);
extern unsigned long fwrite(void* ptr, unsigned long size, unsigned long nmemb, struct FILE* stream);
/* _POSIX_C_SOURCE 200809L */
extern long getline(char** lineptr, unsigned long* n, struct FILE* stream);

/* stdlib.h */
extern void free(void* ptr);
extern void* malloc(unsigned long size);
extern void* realloc(void* ptr, unsigned long size);

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
