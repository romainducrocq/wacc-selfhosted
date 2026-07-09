#ifndef _FRONT_INTERMEDIATE_TAC_REPR_H
#define _FRONT_INTERMEDIATE_TAC_REPR_H

#include "util/c_std.h"

typedef struct CProgram CProgram;
typedef struct TacProgram TacProgram;
typedef struct FrontEndContext FrontEndContext;
typedef struct IdentifierContext IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Three address code representation

unique_ptr_t(TacProgram) represent_three_address_code(
    unique_ptr_t(CProgram) * c_ast, FrontEndContext* frontend, IdentifierContext* identifiers);

#endif
