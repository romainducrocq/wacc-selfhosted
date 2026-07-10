#ifndef _FRONT_INTERMEDIATE_TAC_REPR_H
#define _FRONT_INTERMEDIATE_TAC_REPR_H

#include "util/c_std.h"

struct CProgram;
struct TacProgram;
struct FrontEndContext;
struct IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Three address code representation

unique_ptr_t(struct TacProgram) represent_three_address_code(
    unique_ptr_t(struct CProgram) * c_ast, struct FrontEndContext* frontend, struct IdentifierContext* identifiers);

#endif
