#ifndef _BACK_EMITTER_GAS_CODE_H
#define _BACK_EMITTER_GAS_CODE_H

#include "util/c_std.h"

typedef struct AsmProgram AsmProgram;
typedef struct BackEndContext BackEndContext;
struct FileIoContext;
struct IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Gnu assembler code emission

void emit_gas_code(unique_ptr_t(AsmProgram) * asm_ast, BackEndContext* backend, struct FileIoContext* fileio,
    struct IdentifierContext* identifiers);

#endif
