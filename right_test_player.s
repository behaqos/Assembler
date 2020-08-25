.name "Tester"
.comment "Test comment"

    live    %5
    ld      %5,r2
    st      r3,%5
    add     r1,r6
    sub     r1,r2,r3
    and     r1,%3,r5
    or      r2,r3,r4
    xor     5,r4,r5
    zjmp    5
    ldi     %5,r5,r5
    sti     r5,%4,r5
    fork    %5
    lld     %5,5
    lldi    %5,%5,r5
    lfork   %5
    aff     r5