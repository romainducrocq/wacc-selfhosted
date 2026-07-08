#ifndef _FRONT_PARSER_MESSAGES_H
#define _FRONT_PARSER_MESSAGES_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Errors

#define MESSAGE_LEXER int
#define MSG_unhandled_lexer_error 300
#define MSG_invalid_tok 301
#define MSG_failed_include 302

#define MESSAGE_PARSER int
#define MSG_unhandled_parser_error 400
#define MSG_unexpected_next_tok 401
#define MSG_reached_eof 402
#define MSG_overflow_long_const 403
#define MSG_overflow_ulong_const 404
#define MSG_arr_size_not_int_const 405
#define MSG_case_value_not_int_const 406
#define MSG_expect_unop 407
#define MSG_expect_binop 408
#define MSG_expect_abstract_decltor 409
#define MSG_expect_ptr_unary_factor 410
#define MSG_expect_primary_exp_factor 411
#define MSG_expect_exp 412
#define MSG_for_init_decl_as_fun 413
#define MSG_expect_specifier 414
#define MSG_expect_specifier_list 415
#define MSG_expect_storage_class 416
#define MSG_empty_compound_init 417
#define MSG_derived_fun_decl 418
#define MSG_expect_simple_decltor 419
#define MSG_expect_param_list 420
#define MSG_member_decl_not_auto 421
#define MSG_member_decl_as_fun 422

#endif
