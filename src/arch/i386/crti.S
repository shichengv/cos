.extern _term_init
.extern mac_if
.extern _show_machine_info
.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .init section here. */
	movl $25, %eax
	push %eax
	movl $0x44A, %ecx
	xor %eax, %eax
	movw (%ecx), %ax
	push %eax
	call _term_init

	lea mac_if, %eax
	push %eax
	call _show_machine_info

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	/* gcc will nicely put the contents of crtbegin.o's .fini section here. */