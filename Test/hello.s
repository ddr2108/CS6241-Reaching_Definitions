	.file	"hello.bc"
	.section	.debug_info,"",@progbits
.Lsection_info:
	.section	.debug_abbrev,"",@progbits
.Lsection_abbrev:
	.section	.debug_aranges,"",@progbits
	.section	.debug_macinfo,"",@progbits
	.section	.debug_line,"",@progbits
.Lsection_line:
	.section	.debug_loc,"",@progbits
	.section	.debug_pubnames,"",@progbits
	.section	.debug_pubtypes,"",@progbits
	.section	.debug_str,"MS",@progbits,1
.Linfo_string:
	.section	.debug_ranges,"",@progbits
.Ldebug_range:
	.section	.debug_loc,"",@progbits
.Lsection_debug_loc:
	.file	1 "hello.c"
	.text
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.loc	1 9 0                   # hello.c:9:0
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Ltmp2:
	.cfi_def_cfa_offset 16
.Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp4:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$0, -4(%rbp)
	.loc	1 24 0 prologue_end     # hello.c:24:0
.Ltmp5:
	movl	$9, -28(%rbp)
	.loc	1 25 0                  # hello.c:25:0
	movl	-20(%rbp), %eax
	addl	$9, %eax
	movl	%eax, -24(%rbp)
	.loc	1 28 0                  # hello.c:28:0
	movl	-20(%rbp), %eax
	movl	%eax, -24(%rbp)
	.loc	1 31 0                  # hello.c:31:0
	movl	$10, -24(%rbp)
	jmp	.LBB0_1
	.align	16, 0x90
.LBB0_5:                                # %if.end
                                        #   in Loop: Header=BB0_1 Depth=1
	.loc	1 39 0                  # hello.c:39:0
.Ltmp6:
	movl	$3, -20(%rbp)
	.loc	1 40 0                  # hello.c:40:0
	movl	$5, -24(%rbp)
.Ltmp7:
.LBB0_1:                                # %while.cond
                                        # =>This Inner Loop Header: Depth=1
	.loc	1 32 0                  # hello.c:32:0
	callq	rand
	cltq
	imulq	$1717986919, %rax, %rcx # imm = 0x66666667
	movq	%rcx, %rdx
	shrq	$63, %rdx
	sarq	$35, %rcx
	addl	%edx, %ecx
	imull	$20, %ecx, %ecx
	subl	%ecx, %eax
	cmpl	$5, %eax
	jne	.LBB0_6
# BB#2:                                 # %while.body
                                        #   in Loop: Header=BB0_1 Depth=1
	.loc	1 33 0                  # hello.c:33:0
.Ltmp8:
	movl	-24(%rbp), %eax
	addl	-20(%rbp), %eax
	cmpl	$26, %eax
	movl	%eax, -24(%rbp)
	.loc	1 34 0                  # hello.c:34:0
.Ltmp9:
	jl	.LBB0_4
# BB#3:                                 # %if.then
                                        #   in Loop: Header=BB0_1 Depth=1
	.loc	1 35 0                  # hello.c:35:0
.Ltmp10:
	movl	$15, -24(%rbp)
	jmp	.LBB0_5
.Ltmp11:
	.align	16, 0x90
.LBB0_4:                                # %if.else
                                        #   in Loop: Header=BB0_1 Depth=1
	.loc	1 37 0                  # hello.c:37:0
	movl	$9, -24(%rbp)
	jmp	.LBB0_5
.Ltmp12:
.LBB0_6:                                # %while.end
	.loc	1 43 0                  # hello.c:43:0
	movl	-20(%rbp), %esi
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %ecx
	movl	$.L.str, %edi
	xorl	%eax, %eax
	callq	printf
	.loc	1 44 0                  # hello.c:44:0
	movl	$1, -20(%rbp)
	.loc	1 45 0                  # hello.c:45:0
	movl	$10, -24(%rbp)
	movl	$2, %eax
	.loc	1 46 0                  # hello.c:46:0
	addq	$32, %rsp
	popq	%rbp
	retq
.Ltmp13:
.Ltmp14:
	.size	main, .Ltmp14-main
.Lfunc_end0:
	.cfi_endproc

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d %d %d"
	.size	.L.str, 9

	.type	asd,@object             # @asd
	.comm	asd,4,4
	.text
.Ldebug_end1:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 3.5 (trunk 200694)"
.Linfo_string1:
	.asciz	"hello.c"
.Linfo_string2:
	.asciz	"/home/deep/Documents/Development/CS6241/Homework/Homework 2/Test"
.Linfo_string3:
	.asciz	"asd"
.Linfo_string4:
	.asciz	"int"
.Linfo_string5:
	.asciz	"main"
.Linfo_string6:
	.asciz	"k"
.Linfo_string7:
	.asciz	"i"
.Linfo_string8:
	.asciz	"j"
.Linfo_string9:
	.asciz	"z"
	.section	.debug_info,"",@progbits
.L.debug_info_begin0:
	.long	155                     # Length of Unit
	.short	4                       # DWARF version number
	.long	.Lsection_abbrev        # Offset Into Abbrev. Section
	.byte	8                       # Address Size (in bytes)
	.byte	1                       # Abbrev [1] 0xb:0x94 DW_TAG_compile_unit
	.long	.Linfo_string0          # DW_AT_producer
	.short	12                      # DW_AT_language
	.long	.Linfo_string1          # DW_AT_name
	.long	.Lsection_line          # DW_AT_stmt_list
	.long	.Linfo_string2          # DW_AT_comp_dir
	.quad	0                       # DW_AT_low_pc
	.byte	2                       # Abbrev [2] 0x26:0x15 DW_TAG_variable
	.long	.Linfo_string3          # DW_AT_name
	.long	59                      # DW_AT_type
                                        # DW_AT_external
	.byte	1                       # DW_AT_decl_file
	.byte	8                       # DW_AT_decl_line
	.byte	9                       # DW_AT_location
	.byte	3
	.quad	asd
	.byte	3                       # Abbrev [3] 0x3b:0x7 DW_TAG_base_type
	.long	.Linfo_string4          # DW_AT_name
	.byte	5                       # DW_AT_encoding
	.byte	4                       # DW_AT_byte_size
	.byte	4                       # Abbrev [4] 0x42:0x57 DW_TAG_subprogram
	.long	.Linfo_string5          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	9                       # DW_AT_decl_line
	.long	59                      # DW_AT_type
                                        # DW_AT_external
	.byte	1                       # DW_AT_accessibility
                                        # DW_ACCESS_public
	.quad	.Lfunc_begin0           # DW_AT_low_pc
	.quad	.Lfunc_end0             # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	86
	.byte	5                       # Abbrev [5] 0x60:0xe DW_TAG_variable
	.long	.Linfo_string6          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	21                      # DW_AT_decl_line
	.long	153                     # DW_AT_type
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	112
	.byte	5                       # Abbrev [5] 0x6e:0xe DW_TAG_variable
	.long	.Linfo_string7          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	22                      # DW_AT_decl_line
	.long	59                      # DW_AT_type
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	108
	.byte	5                       # Abbrev [5] 0x7c:0xe DW_TAG_variable
	.long	.Linfo_string8          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	23                      # DW_AT_decl_line
	.long	59                      # DW_AT_type
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	104
	.byte	5                       # Abbrev [5] 0x8a:0xe DW_TAG_variable
	.long	.Linfo_string9          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	24                      # DW_AT_decl_line
	.long	59                      # DW_AT_type
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	100
	.byte	0                       # End Of Children Mark
	.byte	6                       # Abbrev [6] 0x99:0x5 DW_TAG_pointer_type
	.long	59                      # DW_AT_type
	.byte	0                       # End Of Children Mark
.L.debug_info_end0:
	.section	.debug_abbrev,"",@progbits
	.byte	1                       # Abbreviation Code
	.byte	17                      # DW_TAG_compile_unit
	.byte	1                       # DW_CHILDREN_yes
	.byte	37                      # DW_AT_producer
	.byte	14                      # DW_FORM_strp
	.byte	19                      # DW_AT_language
	.byte	5                       # DW_FORM_data2
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	16                      # DW_AT_stmt_list
	.byte	23                      # DW_FORM_sec_offset
	.byte	27                      # DW_AT_comp_dir
	.byte	14                      # DW_FORM_strp
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	2                       # Abbreviation Code
	.byte	52                      # DW_TAG_variable
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	63                      # DW_AT_external
	.byte	25                      # DW_FORM_flag_present
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	2                       # DW_AT_location
	.byte	10                      # DW_FORM_block1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	3                       # Abbreviation Code
	.byte	36                      # DW_TAG_base_type
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	62                      # DW_AT_encoding
	.byte	11                      # DW_FORM_data1
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	4                       # Abbreviation Code
	.byte	46                      # DW_TAG_subprogram
	.byte	1                       # DW_CHILDREN_yes
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	63                      # DW_AT_external
	.byte	25                      # DW_FORM_flag_present
	.byte	50                      # DW_AT_accessibility
	.byte	11                      # DW_FORM_data1
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	1                       # DW_FORM_addr
	.byte	64                      # DW_AT_frame_base
	.byte	10                      # DW_FORM_block1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	5                       # Abbreviation Code
	.byte	52                      # DW_TAG_variable
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	2                       # DW_AT_location
	.byte	10                      # DW_FORM_block1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	6                       # Abbreviation Code
	.byte	15                      # DW_TAG_pointer_type
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	0                       # EOM(3)
	.section	.debug_aranges,"",@progbits
	.long	60                      # Length of ARange Set
	.short	2                       # DWARF Arange version number
	.long	.L.debug_info_begin0    # Offset Into Debug Info Section
	.byte	8                       # Address Size (in bytes)
	.byte	0                       # Segment Size (in bytes)
	.zero	4,255
	.quad	asd
	.quad	4
	.quad	.Lfunc_begin0
.Lset0 = .Ldebug_end1-.Lfunc_begin0
	.quad	.Lset0
	.quad	0                       # ARange terminator
	.quad	0
	.section	.debug_ranges,"",@progbits
.Lgnu_ranges0:
	.section	.debug_pubnames,"",@progbits
.Lset1 = .Lpubnames_end0-.Lpubnames_begin0 # Length of Public Names Info
	.long	.Lset1
.Lpubnames_begin0:
	.short	2                       # DWARF Version
	.long	.L.debug_info_begin0    # Offset of Compilation Unit Info
.Lset2 = .L.debug_info_end0-.L.debug_info_begin0 # Compilation Unit Length
	.long	.Lset2
	.long	66                      # DIE offset
	.asciz	"main"                  # External Name
	.long	38                      # DIE offset
	.asciz	"asd"                   # External Name
	.long	0                       # End Mark
.Lpubnames_end0:
	.section	.debug_pubtypes,"",@progbits
.Lset3 = .Lpubtypes_end0-.Lpubtypes_begin0 # Length of Public Types Info
	.long	.Lset3
.Lpubtypes_begin0:
	.short	2                       # DWARF Version
	.long	.L.debug_info_begin0    # Offset of Compilation Unit Info
.Lset4 = .L.debug_info_end0-.L.debug_info_begin0 # Compilation Unit Length
	.long	.Lset4
	.long	59                      # DIE offset
	.asciz	"int"                   # External Name
	.long	0                       # End Mark
.Lpubtypes_end0:

	.ident	"clang version 3.5 (trunk 200694)"
	.section	".note.GNU-stack","",@progbits
