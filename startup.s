.cpu cortex-m0plus
	.syntax unified
	.thumb
	.fpu softvfp

	.global _vectors, _start, _print_ch

	.section .vectors
	.align 2

	.type _vectors, %object

_vectors:
	.long _stack_top
	.long _start
	.long 0		@ NMI
	.long 0		@ Hardfault
    .long 0		@ Reserved
	.long 0		@ Reserved
	.long 0		@ Reserved
	.long 0		@ Reserved
	.long 0		@ Reserved
	.long 0		@ Reserved
	.long 0		@ Reserved
    .long 0		@ SVC
    .long 0		@ Reserved
    .long 0		@ Reserved
    .long 0		@ PendSV
    .long systick_handler		@ Systick Timer

	.rept 7
	.long 0		@ STM32 Interrupts
	.endr

	.long exti_handler		@ EXTI 4 - 15

	.rept 24
	.long 0		@ STM32 Interrupts
	.endr

	.size _vectors, . - _vectors

	.text
	.align 1

	.type _start, %function

_start:
	ldr r0, =_stack_top
	mov sp, r0
_done:
	cpsie i				@ enabling all interrupts
	b main

	.size _start, . - _start

	.type _print_ch, %function

_print_ch:
	push { r0, r1 } 	@ save old r0 and r1
	mov r1, r0 			@ copy r0 (parameter) to r1
	ldr r0, =#3			@ set r0 to semihosting operation (e.g. #1)
	bkpt 0xab 			@ call semihosting service
	pop { r0, r1 } 		@ restore old r0 and r1
	bx lr 				@ return to calling function

	.size _print_ch, . - _print_ch

	.end
