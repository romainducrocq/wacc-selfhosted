#ifndef _UTIL_STR2T_H
#define _UTIL_STR2T_H

#include <inttypes.h>

#include "util/c_std.h"
#include "util/throw.h"

struct ErrorsContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// String to type

#ifdef __cplusplus
extern "C" {
#endif
int32_t intmax_to_int32(intmax_t intmax);
int64_t intmax_to_int64(intmax_t intmax);
uint32_t uintmax_to_uint32(uintmax_t uintmax);
uint64_t uintmax_to_uint64(uintmax_t uintmax);
void string_to_literal(string_t str_string, vector_t(int8_t) * string_literal);
int32_t string_to_char_ascii(string_t str_char);
int8_t string_bytes_to_int8(vector_t(int8_t) string_literal, size_t byte_at);
int32_t string_bytes_to_int32(vector_t(int8_t) string_literal, size_t byte_at);
int64_t string_bytes_to_int64(vector_t(int8_t) string_literal, size_t byte_at);
string_t string_literal_to_const(vector_t(int8_t) string_literal);
uint64_t dbl_to_binary(double decimal);
error_t string_to_intmax(struct ErrorsContext* ctx, char* str_int, size_t info_at, intmax_t* value);
error_t string_to_uintmax(struct ErrorsContext* ctx, char* str_uint, size_t info_at, uintmax_t* value);
error_t string_to_dbl(struct ErrorsContext* ctx, char* str_dbl, size_t info_at, double* value);
#ifdef __cplusplus
}
#endif

#endif
