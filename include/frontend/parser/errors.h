#ifndef _FRONT_PARSER_ERRORS_H
#define _FRONT_PARSER_ERRORS_H

#include "util/c_std.h"

#include "ast_t.h" // ast

#include "parser/messages.h" // frontend
#include "parser/tokens.h"   // frontend

#include "intermediate/messages.h" // frontend

typedef struct Token Token;
struct FunType;
struct Pointer;
struct Array;
struct Structure;
struct Type;
struct CConst;
struct CUnaryOp;
struct CBinaryOp;
struct CStorageClass;
struct IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Errors

#define MESSAGE_FATAL int
#define MSG_unhandled_fatal_error 0
#define MSG_unsupported_os 1
#define MSG_unsupported_arch 2
#define MSG_unsupported_compiler 3
#define MSG_unsupported_cc_ver 4

#define MESSAGE_ARG int
#define MSG_unhandled_arg_error 100
#define MSG_print_help 101
#define MSG_no_debug_arg 102
#define MSG_invalid_debug_arg 103
#define MSG_no_optim_1_arg 104
#define MSG_invalid_optim_1_arg 105
#define MSG_no_optim_2_arg 106
#define MSG_invalid_optim_2_arg 107
#define MSG_no_input_files_arg 108
#define MSG_no_stdlib_dir_arg 109
#define MSG_no_include_dir_arg 110

#define MESSAGE_UTIL int
#define MSG_unhandled_util_error 200
#define MSG_failed_fread 201
#define MSG_failed_fwrite 202
#define MSG_failed_strtoi 203
#define MSG_failed_strtou 204
#define MSG_failed_strtod 205

char* get_tok_kind_fmt(TOKEN_KIND tok_kind);
char* get_tok_fmt(struct IdentifierContext* ctx, Token* token);
char* get_const_fmt(struct CConst* node);
char* get_storage_class_fmt(struct CStorageClass* node);
char* get_unop_fmt(struct CUnaryOp* node);
char* get_binop_fmt(struct CBinaryOp* node);
char* get_assign_fmt(struct CBinaryOp* node, struct CUnaryOp* unop);
char* get_name_fmt(struct IdentifierContext* ctx, TIdentifier name, string_t* name_fmt);
char* get_struct_name_fmt(struct IdentifierContext* ctx, TIdentifier name, bool is_union, string_t* struct_fmt);
char* get_fun_fmt(struct IdentifierContext* ctx, struct FunType* fun_type, string_t* fun_fmt);
char* get_ptr_fmt(struct IdentifierContext* ctx, struct Pointer* ptr_type, string_t* ptr_fmt);
char* get_arr_fmt(struct IdentifierContext* ctx, struct Array* arr_type, string_t* arr_fmt);
char* get_struct_fmt(struct IdentifierContext* ctx, struct Structure* struct_type, string_t* struct_fmt);
char* get_type_fmt(struct IdentifierContext* ctx, struct Type* type, string_t* type_fmt);
#define str_get_fmt(T, ...) get_##T##_fmt(ctx->identifiers, __VA_ARGS__)
#define str_fmt_tok(X) str_get_fmt(tok, X)
#define str_fmt_name(X, Y) str_get_fmt(name, X, Y)
#define str_fmt_struct_name(X, Y, Z) str_get_fmt(struct_name, X, Y, Z)
#define str_fmt_fun(X, Y) str_get_fmt(fun, X, Y)
#define str_fmt_ptr(X, Y) str_get_fmt(ptr, X, Y)
#define str_fmt_arr(X, Y) str_get_fmt(arr, X, Y)
#define str_fmt_struct(X, Y) str_get_fmt(struct, X, Y)
#define str_fmt_type(X, Y) str_get_fmt(type, X, Y)

char* get_fatal_msg(MESSAGE_FATAL msg);
char* get_arg_msg(MESSAGE_ARG msg);
char* get_util_msg(MESSAGE_UTIL msg);
char* get_lexer_msg(MESSAGE_LEXER msg);
char* get_parser_msg(MESSAGE_PARSER msg);
char* get_semantic_msg(MESSAGE_SEMANTIC msg);
#define GET_MESSAGE(E, N) get_##E##_msg(N), #N
#define GET_MESSAGE_0(E, N) GET_MESSAGE(E, N), "", "", ""
#define GET_MESSAGE_1(E, N, X) GET_MESSAGE(E, N), "", "", X
#define GET_MESSAGE_2(E, N, X, Y) GET_MESSAGE(E, N), "", X, Y
#define GET_MESSAGE_3(E, N, X, Y, Z) GET_MESSAGE(E, N), X, Y, Z
#define GET_FATAL_MSG(N, ...) GET_MESSAGE_##N(fatal, __VA_ARGS__)
#define GET_ARG_MSG(N, ...) GET_MESSAGE_##N(arg, __VA_ARGS__)
#define GET_UTIL_MSG(N, ...) GET_MESSAGE_##N(util, __VA_ARGS__)
#define GET_LEXER_MSG(N, ...) GET_MESSAGE_##N(lexer, __VA_ARGS__)
#define GET_PARSER_MSG(N, ...) GET_MESSAGE_##N(parser, __VA_ARGS__)
#define GET_SEMANTIC_MSG(N, ...) GET_MESSAGE_##N(semantic, __VA_ARGS__)

#define STRINGIFY(X) #X
#define GET_VERSION(X, Y, Z) STRINGIFY(X), STRINGIFY(Y), STRINGIFY(Z)
#define GCC_VERSION GET_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#define CLANG_VERSION GET_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)

#endif
