.name "Tester"
.comment "Test comment"

#oreo:
#    or      2,%3,r4

#jumping: zjmp    %5
    ldi     %:jumping,r5,r5
    sti     r5,%4,r5
    fork    %5
    lld     %5,5
    lldi    %5,%5,r5
    lfork   %5
    aff     r5
    #    live    %5
    #    ld      %5,r2
    #    st      r3,5
    #    add     r1,r6,r7
    #    sub     r1,r6,r7
    #    and     5,5,r7
   # core:
     # xorrrrr:
       #   xor     55,%5,r5