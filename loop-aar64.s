.text
.globl    _start
start = 0
max = 31

_start:
        mov     x19, start
        mov     x22, 10
loop:
	/* load msg into register 1 in order to print the intial 0 */
        adr     x1, msg
	/* different way of loading the message length, using MOV */
        mov     x2, len
        mov     x0, 1
	/*move the correct sys_write value into r8 */
        mov     x8, 64
	/* make the system call */
        svc     0

	/*load the message in a new memory location */
        adr     x23, msg
	/*not sure how to use the inc call like seen in x86, so we load x19, with it's same data + 1 */
        add     x19, x19, 1
	/* in order to figure out if we need a ten's value, we divide our incrementing value,
	stored in x19, with the value of 10, which we stored in x22, and we will place the quotient value
	in x20,
	e.g first loop will be 1 / 10 = 0 stored in x20 */
        udiv    x20, x19, x22
	/* calculate the remainder for the current value, load it into x21
	e.g first loop will be 1 - (10 * 1) */
        msub    x21, x22, x20, x19
	cmp     x23, 0
        beq     skip
	add x20, x20, 0x30
	strb w20, [x23,6]

skip:
	/*convert ones character to ascii for printing*/
        add     x21, x21, 0x30
	/* add it to the 7th byte of msg */
        strb    w21, [x23,7]
        cmp     x19, max
        bne     loop
        mov     x0, 0
        mov     x8, 93
        svc     0
.data
        msg: .ascii  "Loop:  0\n"
        len = . - msg
