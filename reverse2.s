
	.text
	.syntax unified
	.thumb
	.cpu cortex-m0plus
	.type reverse_bits, %function
	.global reverse_bits

reverse_bits:
	movs r3, r0
	push {lr}
	movs r2, #32
	movs r0, #0
.L2:
	lsls r0, r0, #1
	lsrs r3, r3, #1
	adc r0, r0, #0
	subs r2, r2, #1
	bne .L2
	pop {pc}
