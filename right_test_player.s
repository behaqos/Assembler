.name "Tester"
.comment "Test comment"

# Не работает парсинг операции zjmp.
#TODO путается лейбл с именем при парсинге. Исправить как можно скорее.
jumping: zjmp    %5
    ldi     %5,r5,r5
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
      #  or      2,%3,r4
     # xorrrrr:
       #   xor     55,%5,r5