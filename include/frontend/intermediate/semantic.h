#ifndef _FRONT_INTERMEDIATE_SEMANTIC_H
#define _FRONT_INTERMEDIATE_SEMANTIC_H

#include "util/throw.h"

typedef struct CProgram CProgram;
struct ErrorsContext;
typedef struct FrontEndContext FrontEndContext;
struct IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Semantic analysis

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Type checking
// Loop labeling
// Identifier resolution

error_t analyze_semantic(
    CProgram* node, struct ErrorsContext* errors, FrontEndContext* frontend, struct IdentifierContext* identifiers);

#endif
