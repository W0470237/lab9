	.cpu cortex-m0plus
	.syntax unified
	.thumb
	.fpu softvfp

	.global _vectors, _start

	.section .vectors
	.align 2

	.type _vectors, %object
_vectors:
	.long _stack_top
	.long _start

	.rept 46
	.long 0
	.endr

	.size _vectors, . - _vectors

	.text
	.align 1

	.type _start, %function

_start:
	ldr r0, =_stack_top
	mov sp, r0

_done:
	b main

	.size _start, . - _start

	.end
