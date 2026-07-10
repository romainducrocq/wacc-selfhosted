#ifndef _FRONT_INTERMEDIATE_IDENTS_H
#define _FRONT_INTERMEDIATE_IDENTS_H

#include "ast_t.h" // ast

#include "intermediate/labels.h" // frontend

struct CExp;
struct IdentifierContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Identifiers

TIdentifier rslv_label_identifier(struct IdentifierContext* ctx, TIdentifier label);
TIdentifier rslv_var_identifier(struct IdentifierContext* ctx, TIdentifier variable);
TIdentifier rslv_struct_tag(struct IdentifierContext* ctx, TIdentifier structure);
TIdentifier repr_label_identifier(struct IdentifierContext* ctx, LABEL_KIND label_kind);
TIdentifier repr_loop_identifier(struct IdentifierContext* ctx, LABEL_KIND label_kind, TIdentifier target);
TIdentifier repr_case_identifier(struct IdentifierContext* ctx, TIdentifier target, bool is_label, unsigned long i);
TIdentifier repr_var_identifier(struct IdentifierContext* ctx, struct CExp* node);

#endif
