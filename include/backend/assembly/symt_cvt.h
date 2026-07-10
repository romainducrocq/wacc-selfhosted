#ifndef _BACK_ASSEMBLY_SYMT_CVT_H
#define _BACK_ASSEMBLY_SYMT_CVT_H

#include "util/c_std.h"

#include "ast_t.h" // ast

struct Type;
struct AssemblyType;
typedef struct AsmProgram AsmProgram;
struct BackEndContext;
struct FrontEndContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Symbol table conversion

TInt gen_type_alignment(struct FrontEndContext* ctx, struct Type* type);
shared_ptr_t(struct AssemblyType) cvt_backend_asm_type(struct FrontEndContext* ctx, TIdentifier name);
void convert_symbol_table(AsmProgram* node, struct BackEndContext* backend, struct FrontEndContext* frontend);

#endif
