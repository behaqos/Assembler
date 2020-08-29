.name "Tester"
.comment "Test comment"

    live    %1
    ld      %2,r3
    st      r4,5
    add     r6,r7,r8
    sub     r9,r10,r11
    and     12,13,r14
oreo:
    or      15,%16,r16
jumping: zjmp    %17
    ldi     %:jumping,%16,r16
    sti     r16,18,r16
    fork    %19
    lld     %20,r16
    lldi    %21,%22,r16
    lfork   %23
    aff     r16

core:
xorrrrr:
    xor     24,%25,r16

    //FIXME не показывает метки. Метки должен присваивать операции, которая идёт следующей или после двоеточия.