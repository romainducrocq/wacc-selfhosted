#ifndef _FRONT_PARSER_LEXER_H
#define _FRONT_PARSER_LEXER_H

#include "util/c_std.h"
#include "util/throw.h"

#include "ast_t.h" // ast

#include "parser/tokens.h" // frontend

struct ErrorsContext;
struct FileIoContext;
typedef struct IdentifierContext IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Lexer

typedef struct Token {
    TOKEN_KIND tok_kind;
    TIdentifier tok;
    size_t info_at;
} Token;

#ifdef __cplusplus
extern "C" {
#endif
error_t lex_c_code(string_t filename, vector_t(char*) * includedirs, vector_t(char*) * stdlibdirs,
    struct ErrorsContext* errors, struct FileIoContext* fileio, IdentifierContext* identifiers,
    vector_t(Token) * tokens);
#ifdef __cplusplus
}
#endif

#endif
