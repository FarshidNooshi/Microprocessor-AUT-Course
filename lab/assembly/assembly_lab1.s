PIO_PER  equ  0x400E0E00	
PIO_SODR equ  0x400E0E30
PIO_CODR equ  0x400E0E34
PIO_OER  equ  0x400E0E10 
PIO_ISR  equ  0x400E0E4C
		
		area labcode, code, readonly
		export __main
		entry

__main
	bl pio_enb1
	bl pio_enb2
	bl pio_enb3
	
begin
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2,#2_100000
	beq loop
	b begin
	
	
finish
	bl turn_off_led1
	bl turn_off_led2
	bl turn_off_led3
	b begin

loop
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish

	bl turn_on_led1
	bl turn_on_led2
	bl turn_on_led3
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_off_led1
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_on_led1
	
	bl turn_off_led2
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_off_led1
	
	bl turn_off_led3
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_on_led2
	
	bl turn_on_led1
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_off_led1
	
	bl delay
	
	ldr r1, =PIO_ISR
	ldr r2, [r1]
	cmp r2, #2_1000000
	beq finish
	
	bl turn_off_led2
	
	bl loop

pio_enb1

	mov r4, #2_1
	
	ldr r5,=PIO_PER
	str r4, [r5]
	
	ldr r5,=PIO_OER
	str r4, [r5]
	
	bx lr
	
pio_enb2
	mov r4, #2_10
	
	ldr r5,=PIO_PER
	str r4, [r5]
	
	ldr r5,=PIO_OER
	str r4, [r5]
	
	bx lr
	
pio_enb3
	mov r4, #2_100
	
	ldr r5,=PIO_PER
	str r4, [r5]
	
	ldr r5,=PIO_OER
	str r4, [r5]
	
	bx lr
	
turn_on_led1
	mov r4, #2_1
	
	ldr r5,=PIO_SODR
	str r4, [r5]
	
	bx lr
	
turn_on_led2
	mov r4, #2_10
	
	ldr r5,=PIO_SODR
	str r4, [r5]
	
	bx lr
	
turn_on_led3
	mov r4, #2_100
	
	ldr r5,=PIO_SODR
	str r4, [r5]
	
	bx lr
	
turn_off_led1
	mov r4, #2_1
	
	ldr r5,=PIO_CODR
	str r4, [r5]
	
	bx lr
	
turn_off_led2
	mov r4, #2_10
	
	ldr r5,=PIO_CODR
	str r4, [r5]
	
	bx lr

turn_off_led3
	mov r4, #2_100
	
	ldr r5,=PIO_CODR
	str r4, [r5]
	
	bx lr
	
delay
	mov r4, #0
	ldr r5, =0x00C0000
	
loop_delay
	ADD r4,r4,#1
	
	cmp r4,r5
	bne loop_delay
	
	bx lr
	
	end