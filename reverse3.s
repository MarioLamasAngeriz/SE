
	.text
	.syntax unified
	.thumb
	.cpu cortex-m0plus
	.type reverse_bits, %function
	.global reverse_bits

reverse_bits:
	mov r1, #0x55555555
	and r2, r0, r1
	lsrs r0, r0, #1
	and r0, r0, r1
	lsls r2, r2, #1
	orr r0, r0, r2

	mov r1, #0x33333333
	and r2, r0, r1
	lsrs r0, r0, #2
	and r0, r0, r1
	lsls r2, r2, #2
	orr r0, r0, r2

	mov r1, #0x0F0F0F0F
	and r2, r0, r1
	lsrs r0, r0, #4
	and r0, r0, r1
	lsls r2, r2, #4
	orr r0, r0, r2

	rev r0, r0


	//ajustes menores para q funcione
