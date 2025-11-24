
	.text
	.syntax unified
	.thumb
	.cpu cortex-m0plus
	.type reverse_bits, %function
	.global reverse_bits

reverse_bits:
	push {r4, lr}
	
	ldr r1, =#0x55555555
	mov r2, r0
	ands r2, r1
	lsrs r0, r0, #1
	ands r0, r1
	lsls r2, r2, #1
	orrs r0, r2

	ldr r1, =#0x33333333
	mov r2, r0
	ands r2, r1
	lsrs r0, r0, #2
	ands r0, r1
	lsls r2, r2, #2
	orrs r0, r2

	ldr r1, =#0x0F0F0F0F
	mov r2, r0
	ands r2, r1
	lsrs r0, r0, #4
	ands r0, r1
	lsls r2, r2, #4
	orrs r0, r2

	rev r0, r0

	pop {r4, pc}


