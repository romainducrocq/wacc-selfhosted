#ifndef _FRONT_PARSER_PARSER_H
#define _FRONT_PARSER_PARSER_H

#include "util/c_std.h"
#include "util/throw.h"

typedef struct Token Token;
typedef struct CProgram CProgram;
struct ErrorsContext;
typedef struct IdentifierContext IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parser

error_t parse_tokens(vector_t(Token) * tokens, struct ErrorsContext* errors, IdentifierContext* identifiers,
    unique_ptr_t(CProgram) * c_ast);

#endif
