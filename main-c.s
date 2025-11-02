	.cpu cortex-m0plus
	.arch armv6s-m
	.fpu softvfp
	.eabi_attribute 23, 1	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 1	@ Tag_ABI_enum_size
	.eabi_attribute 30, 2	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 4	@ Tag_ABI_PCS_wchar_t
	.file	"main-c.c"
@ GNU C17 (Arch Repository) version 14.2.0 (arm-none-eabi)
@	compiled by GNU C version 14.2.1 20240910, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.27-GMP

@ warning: MPFR header version 4.2.1 differs from library version 4.2.2.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed: -mthumb -mcpu=cortex-m0plus -mfloat-abi=soft -march=armv6s-m -Ofast
	.text
	.align	1
	.p2align 2,,3
	.global	reverse_int
	.syntax unified
	.code	16
	.thumb_func
	.type	reverse_int, %function
reverse_int:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, r0	@ in, tmp122
	push	{r4, lr}	@
@ main-c.c:9: unsigned int reverse_int(unsigned int in) {
	movs	r2, #32	@ ivtmp_16,
@ main-c.c:10: 	unsigned int out = 0;
	movs	r0, #0	@ <retval>,
@ main-c.c:13: 		out |= in & 1;
	movs	r4, #1	@ tmp119,
.L2:
	movs	r1, r4	@ _1, tmp119
@ main-c.c:12: 		out = out << 1;
	lsls	r0, r0, #1	@ out, <retval>,
@ main-c.c:13: 		out |= in & 1;
	ands	r1, r3	@ _1, in
@ main-c.c:11: 	for (unsigned int i=0; i<32; i++) {
	subs	r2, r2, #1	@ ivtmp_16,
@ main-c.c:13: 		out |= in & 1;
	orrs	r0, r1	@ <retval>, _1
@ main-c.c:14: 		in = in >> 1;
	lsrs	r3, r3, #1	@ in, in,
@ main-c.c:11: 	for (unsigned int i=0; i<32; i++) {
	cmp	r2, #0	@ ivtmp_16,
	bne	.L2		@,
@ main-c.c:17: }
	@ sp needed	@
	pop	{r4, pc}
	.size	reverse_int, .-reverse_int
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"N\303\272mero en binario: 0b\000"
	.align	2
.LC2:
	.ascii	"1\000"
	.align	2
.LC4:
	.ascii	"0\000"
	.align	2
.LC6:
	.ascii	" \000"
	.align	2
.LC8:
	.ascii	"\015\012\000"
	.text
	.align	1
	.p2align 2,,3
	.global	imprimir_en_binario
	.syntax unified
	.code	16
	.thumb_func
	.type	imprimir_en_binario, %function
imprimir_en_binario:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}	@
	mov	lr, r9	@,
	mov	r7, r8	@,
	movs	r5, r0	@ in, tmp132
	push	{r7, lr}	@
@ main-c.c:20: 	PRINTF("Número en binario: 0b");
	ldr	r0, .L15	@ tmp119,
	bl	DbgConsole_Printf		@
	ldr	r3, .L15+4	@ tmp129,
@ main-c.c:21: 	for (int i = 31; i >= 0; i--) {
	movs	r4, #31	@ i,
	mov	r9, r3	@ tmp129, tmp129
@ main-c.c:25: 			PRINTF("0");
	ldr	r3, .L15+8	@ tmp130,
@ main-c.c:22: 		if (in & (1u << i)) {
	movs	r6, #1	@ tmp121,
@ main-c.c:25: 			PRINTF("0");
	mov	r8, r3	@ tmp130, tmp130
@ main-c.c:23: 			PRINTF("1");
	ldr	r7, .L15+12	@ tmp131,
.L11:
@ main-c.c:22: 		if (in & (1u << i)) {
	movs	r3, r5	@ _1, in
	lsrs	r3, r3, r4	@ _1, _1, i
@ main-c.c:22: 		if (in & (1u << i)) {
	tst	r6, r3	@ tmp121, _1
	beq	.L6		@,
.L13:
@ main-c.c:23: 			PRINTF("1");
	movs	r0, r7	@, tmp131
	bl	DbgConsole_Printf		@
@ main-c.c:27: 		if (i % 8 == 0)
	lsls	r3, r4, #29	@ tmp133, i,
	beq	.L8		@,
.L14:
@ main-c.c:22: 		if (in & (1u << i)) {
	movs	r3, r5	@ _1, in
@ main-c.c:21: 	for (int i = 31; i >= 0; i--) {
	subs	r4, r4, #1	@ i,
@ main-c.c:22: 		if (in & (1u << i)) {
	lsrs	r3, r3, r4	@ _1, _1, i
@ main-c.c:22: 		if (in & (1u << i)) {
	tst	r6, r3	@ tmp121, _1
	bne	.L13		@,
.L6:
@ main-c.c:25: 			PRINTF("0");
	mov	r0, r8	@, tmp130
	bl	DbgConsole_Printf		@
@ main-c.c:27: 		if (i % 8 == 0)
	lsls	r3, r4, #29	@ tmp133, i,
	bne	.L14		@,
.L8:
@ main-c.c:28: 			PRINTF(" ");
	mov	r0, r9	@, tmp129
	bl	DbgConsole_Printf		@
@ main-c.c:21: 	for (int i = 31; i >= 0; i--) {
	subs	r4, r4, #1	@ i, i
	bcs	.L11		@,
@ main-c.c:30: 	PRINTF("\r\n");
	ldr	r0, .L15+16	@ tmp128,
	bl	DbgConsole_Printf		@
@ main-c.c:31: }
	@ sp needed	@
	pop	{r6, r7}
	mov	r9, r7
	mov	r8, r6
	pop	{r3, r4, r5, r6, r7, pc}
.L16:
	.align	2
.L15:
	.word	.LC0
	.word	.LC6
	.word	.LC4
	.word	.LC2
	.word	.LC8
	.size	imprimir_en_binario, .-imprimir_en_binario
	.align	1
	.p2align 2,,3
	.global	systic_conf
	.syntax unified
	.code	16
	.thumb_func
	.type	systic_conf, %function
systic_conf:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ main-c.c:34: 	SysTick->CTRL = 0;
	movs	r2, #0	@ tmp115,
@ main-c.c:38: }
	@ sp needed	@
@ main-c.c:34: 	SysTick->CTRL = 0;
	ldr	r3, .L18	@ tmp114,
@ main-c.c:35: 	SysTick->LOAD = 0xFFFFFF;
	ldr	r1, .L18+4	@ tmp117,
@ main-c.c:34: 	SysTick->CTRL = 0;
	str	r2, [r3]	@ tmp115, MEM[(struct SysTick_Type *)3758153744B].CTRL
@ main-c.c:35: 	SysTick->LOAD = 0xFFFFFF;
	str	r1, [r3, #4]	@ tmp117, MEM[(struct SysTick_Type *)3758153744B].LOAD
@ main-c.c:36: 	SysTick->VAL = 0;
	str	r2, [r3, #8]	@ tmp115, MEM[(struct SysTick_Type *)3758153744B].VAL
@ main-c.c:37: 	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	adds	r2, r2, #1	@ tmp121,
	str	r2, [r3]	@ tmp121, MEM[(struct SysTick_Type *)3758153744B].CTRL
@ main-c.c:38: }
	bx	lr
.L19:
	.align	2
.L18:
	.word	-536813552
	.word	16777215
	.size	systic_conf, .-systic_conf
	.align	1
	.p2align 2,,3
	.global	systic_get
	.syntax unified
	.code	16
	.thumb_func
	.type	systic_get, %function
systic_get:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r3, .L21	@ tmp116,
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r0, .L21+4	@ tmp118,
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r3, [r3, #8]	@ _1, MEM[(struct SysTick_Type *)3758153744B].VAL
@ main-c.c:42: }
	@ sp needed	@
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	subs	r0, r0, r3	@ _3, tmp118, _1
@ main-c.c:42: }
	bx	lr
.L22:
	.align	2
.L21:
	.word	-536813552
	.word	16777215
	.size	systic_get, .-systic_get
	.section	.rodata.str1.4
	.align	2
.LC10:
	.ascii	"Introduce un n\303\272mero enteiro, max 9 dixitos\015"
	.ascii	"\012\000"
	.align	2
.LC12:
	.ascii	"\015\012Numero a invertir bit a bit: %d\015\012\000"
	.align	2
.LC14:
	.ascii	"Numero invertido bit a bit: %d\015\012\000"
	.align	2
.LC16:
	.ascii	"N\303\272mero de ticks de SysTick (equivalentes a c"
	.ascii	"iclos de CPU por tener ambos la misma frecuencia) \015"
	.ascii	"\012para la ejecuci\303\263n de la funci\303\263n d"
	.ascii	"e inversion de bits = %d\015\012\000"
	.section	.text.startup,"ax",%progbits
	.align	1
	.p2align 2,,3
	.global	main
	.syntax unified
	.code	16
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}	@
	sub	sp, sp, #20	@,,
@ main-c.c:53: 	BOARD_InitPins();
	bl	BOARD_InitPins		@
@ main-c.c:54: 	BOARD_BootClockRUN();
	bl	BOARD_BootClockRUN		@
@ main-c.c:55: 	BOARD_InitDebugConsole();
	bl	BOARD_InitDebugConsole		@
@ main-c.c:57: 	PRINTF("Introduce un número enteiro, max 9 dixitos\r\n");
	ldr	r0, .L35	@ tmp129,
	bl	DbgConsole_Printf		@
@ main-c.c:64: 		num_str[i++] = ch;
	mov	r3, sp	@ tmp164,
@ main-c.c:47: 	int i = 0;
	movs	r4, #0	@ i,
@ main-c.c:61: 		PUTCHAR(ch);
	movs	r6, #255	@ tmp130,
@ main-c.c:64: 		num_str[i++] = ch;
	adds	r7, r3, #3	@ tmp157, tmp164,
.L25:
@ main-c.c:60: 		ch = GETCHAR();
	bl	DbgConsole_Getchar		@
@ main-c.c:60: 		ch = GETCHAR();
	uxtb	r5, r0	@ ch, _1
@ main-c.c:61: 		PUTCHAR(ch);
	ands	r0, r6	@ _43, tmp130
	bl	DbgConsole_Putchar		@
@ main-c.c:62: 		if (ch == '\n' || ch == '\r')
	cmp	r5, #10	@ ch,
	beq	.L24		@,
@ main-c.c:62: 		if (ch == '\n' || ch == '\r')
	cmp	r5, #13	@ ch,
	beq	.L24		@,
@ main-c.c:64: 		num_str[i++] = ch;
	adds	r4, r4, #1	@ i,
@ main-c.c:64: 		num_str[i++] = ch;
	strb	r5, [r7, r4]	@ ch, MEM[(char *)&num_str + 4294967295B + _54 * 1]
@ main-c.c:59: 	while (i < 9) {
	cmp	r4, #9	@ i,
	bne	.L25		@,
.L24:
@ main-c.c:67: 	num_str[i] = '\0';
	mov	r3, sp	@ tmp165,
	movs	r6, #0	@ tmp138,
	adds	r3, r3, #4	@ tmp166,
@ main-c.c:69: 	unsigned int num = atoi(num_str);
	movs	r0, r3	@ tmp167, tmp166
@ main-c.c:67: 	num_str[i] = '\0';
	strb	r6, [r3, r4]	@ tmp138, num_str[i_37]
@ main-c.c:69: 	unsigned int num = atoi(num_str);
	bl	atoi		@
	movs	r5, r0	@ num, tmp159
@ main-c.c:70: 	PRINTF("\r\nNumero a invertir bit a bit: %d\r\n", num);
	ldr	r0, .L35+4	@ tmp141,
	movs	r1, r5	@, num
	bl	DbgConsole_Printf		@
@ main-c.c:71: 	imprimir_en_binario(num);
	movs	r0, r5	@, num
	bl	imprimir_en_binario		@
@ main-c.c:34: 	SysTick->CTRL = 0;
	ldr	r3, .L35+8	@ tmp142,
@ main-c.c:35: 	SysTick->LOAD = 0xFFFFFF;
	ldr	r2, .L35+12	@ tmp145,
@ main-c.c:34: 	SysTick->CTRL = 0;
	str	r6, [r3]	@ tmp138, MEM[(struct SysTick_Type *)3758153744B].CTRL
@ main-c.c:35: 	SysTick->LOAD = 0xFFFFFF;
	str	r2, [r3, #4]	@ tmp145, MEM[(struct SysTick_Type *)3758153744B].LOAD
@ main-c.c:37: 	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	movs	r2, #1	@ tmp149,
@ main-c.c:36: 	SysTick->VAL = 0;
	str	r6, [r3, #8]	@ tmp138, MEM[(struct SysTick_Type *)3758153744B].VAL
@ main-c.c:37: 	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	str	r2, [r3]	@ tmp149, MEM[(struct SysTick_Type *)3758153744B].CTRL
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r1, [r3, #8]	@ _36, MEM[(struct SysTick_Type *)3758153744B].VAL
@ main-c.c:10: 	unsigned int out = 0;
	movs	r4, #0	@ out,
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	movs	r3, #32	@ ivtmp_4,
@ main-c.c:13: 		out |= in & 1;
	movs	r0, #1	@ tmp151,
.L26:
	movs	r2, r0	@ _31, tmp151
@ main-c.c:12: 		out = out << 1;
	lsls	r4, r4, #1	@ out, out,
@ main-c.c:13: 		out |= in & 1;
	ands	r2, r5	@ _31, num
@ main-c.c:11: 	for (unsigned int i=0; i<32; i++) {
	subs	r3, r3, #1	@ ivtmp_4,
@ main-c.c:13: 		out |= in & 1;
	orrs	r4, r2	@ out, _31
@ main-c.c:14: 		in = in >> 1;
	lsrs	r5, r5, #1	@ num, num,
@ main-c.c:11: 	for (unsigned int i=0; i<32; i++) {
	cmp	r3, #0	@ ivtmp_4,
	bne	.L26		@,
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r3, .L35+8	@ tmp153,
@ main-c.c:81: 	PRINTF("Numero invertido bit a bit: %d\r\n", reversed);
	ldr	r0, .L35+16	@ tmp154,
@ main-c.c:41: 	return 0xFFFFFF - SysTick->VAL;
	ldr	r5, [r3, #8]	@ _21, MEM[(struct SysTick_Type *)3758153744B].VAL
@ main-c.c:79: 	ciclosCPU = fin - inicio; // realmente son ticks de systick pero como la cpu tiene la misma frecuencia que el systick son equivalentes
	subs	r5, r1, r5	@ ciclosCPU, _36, _21
@ main-c.c:81: 	PRINTF("Numero invertido bit a bit: %d\r\n", reversed);
	movs	r1, r4	@, out
	bl	DbgConsole_Printf		@
@ main-c.c:82: 	imprimir_en_binario(reversed);
	movs	r0, r4	@, out
	bl	imprimir_en_binario		@
@ main-c.c:84: 	PRINTF("Número de ticks de SysTick (equivalentes a ciclos de CPU por tener ambos la misma frecuencia) \r\n"
	movs	r1, r5	@, ciclosCPU
	ldr	r0, .L35+20	@ tmp155,
	bl	DbgConsole_Printf		@
@ main-c.c:86: }
	movs	r0, #0	@,
	add	sp, sp, #20	@,,
	@ sp needed	@
	pop	{r4, r5, r6, r7, pc}
.L36:
	.align	2
.L35:
	.word	.LC10
	.word	.LC12
	.word	-536813552
	.word	16777215
	.word	.LC14
	.word	.LC16
	.size	main, .-main
	.ident	"GCC: (Arch Repository) 14.2.0"
