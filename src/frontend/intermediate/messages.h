#ifndef _FRONT_INTERMEDIATE_MESSAGES_H
#define _FRONT_INTERMEDIATE_MESSAGES_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Errors

#define MESSAGE_SEMANTIC int
#define MSG_unhandled_semantic_error 500
#define MSG_incomplete_arr 501
#define MSG_joint_ptr_mismatch 502
#define MSG_fun_used_as_var 503
#define MSG_illegal_cast 504
#define MSG_invalid_unary_op 505
#define MSG_invalid_binary_op 506
#define MSG_invalid_binary_ops 507
#define MSG_assign_to_void 508
#define MSG_assign_to_rvalue 509
#define MSG_invalid_condition 510
#define MSG_invalid_ternary_op 511
#define MSG_var_used_as_fun 512
#define MSG_call_with_wrong_argc 513
#define MSG_deref_not_ptr 514
#define MSG_addrof_rvalue 515
#define MSG_invalid_subscript 516
#define MSG_sizeof_incomplete 517
#define MSG_dot_not_struct 518
#define MSG_member_not_in_struct 519
#define MSG_arrow_not_struct_ptr 520
#define MSG_arrow_incomplete 521
#define MSG_exp_incomplete 522
#define MSG_ret_value_in_void_fun 523
#define MSG_no_ret_value_in_fun 524
#define MSG_invalid_if 525
#define MSG_invalid_while 526
#define MSG_invalid_do_while 527
#define MSG_invalid_for 528
#define MSG_invalid_switch 529
#define MSG_duplicate_case_value 530
#define MSG_string_init_not_char_arr 531
#define MSG_string_init_overflow 532
#define MSG_arr_init_overflow 533
#define MSG_struct_init_overflow 534
#define MSG_ret_arr 535
#define MSG_ret_incomplete 536
#define MSG_void_param 537
#define MSG_incomplete_param 538
#define MSG_redecl_fun_conflict 539
#define MSG_redef_fun 540
#define MSG_redecl_static_conflict 541
#define MSG_static_ptr_init_not_int 542
#define MSG_static_ptr_init_not_null 543
#define MSG_agg_init_with_single 544
#define MSG_static_ptr_init_string 545
#define MSG_static_init_not_const 546
#define MSG_scalar_init_with_compound 547
#define MSG_void_var_decl 548
#define MSG_incomplete_var_decl 549
#define MSG_redecl_var_conflict 550
#define MSG_redecl_var_storage 551
#define MSG_redef_extern_var 552
#define MSG_duplicate_member_decl 553
#define MSG_incomplete_member_decl 554
#define MSG_redecl_struct_in_scope 555
#define MSG_case_out_of_switch 556
#define MSG_default_out_of_switch 557
#define MSG_multiple_default 558
#define MSG_break_out_of_loop 559
#define MSG_continue_out_of_loop 560
#define MSG_undef_goto_target 561
#define MSG_redecl_struct_conflict 562
#define MSG_undef_struct_in_scope 563
#define MSG_undecl_var_in_scope 564
#define MSG_undecl_fun_in_scope 565
#define MSG_for_init_decl_not_auto 566
#define MSG_redef_label_in_scope 567
#define MSG_redecl_var_in_scope 568
#define MSG_def_nested_fun 569
#define MSG_decl_nested_static_fun 570
#define MSG_redecl_fun_in_scope 571

#endif
