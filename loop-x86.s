_start:
    mov     $start,%r15         /*Loop index */
    mov     $1,%r12

loop:                           /* ... body of the loop ... do something useful here ... */
    mov     $len,%rdx           /*Message length for loop phrase only*/
    mov     $msg,%rsi           /*Message location*/

    mov     %r15,%r13           /*Copys register 15 into register 13*/
    cmp     $10,%r13            /*If counter hits 10 call function to increment 10's value*/
    je      tens
    add     $48,%r13            /*Add 48 to get ascii value */
    mov     %r13b, msg+7        /*Move one byte of register into msg string at position 7*/

    mov     $1,%rax             /*Print call*/
    syscall

    inc     %r15                /*Increment index */
    jmp     check               /*Check Values*/

tens:
    mov     $0,%r15             /*Reset single digit value*/
    mov     %r12,%r14           /*Make copy of tens register*/
    add     $48,%r14            /*Same process from loop except will tens digit*/
    mov     %r14b, msg+6
    inc     %r12
    jmp     check                /*Loop if we're not */

check:
    mov     $10, %rax           /*Move 10 to rax for multiply*/
    mov     %r12,%rbx           /*Copy active increment registers*/
    mov     %r15,%rbp
    mul     %rbx
    add     %rax,%rbp           /*Add copy registers to compare with max*/
    cmp     $max,%rbp
    je      quit
    jmp     loop

quit:

    mov     $0,%rdi             /* exit status */
    mov     $60,%rax            /* syscall sys_exit */
    syscall

.data                           /*for storing constant data otherwise seg fault*/
msg:  .ascii            "Loop :  \n"
.set len, . - msg

