#ifndef _BACK_ASSEMBLY_STACK_FIX_H
#define _BACK_ASSEMBLY_STACK_FIX_H

#include "util/c_std.h"

#include "ast_t.h" // ast

struct AsmInstruction;
struct AsmProgram;
struct BackEndContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Stack fix

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pseudo register replacement
// Instruction fix up

unique_ptr_t(AsmInstruction) alloc_stack_bytes(TLong byte);
void fix_stack(struct AsmProgram* node, struct BackEndContext* backend);

#endif
