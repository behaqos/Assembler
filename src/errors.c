#include "../includes/asm.h"

void                error_printf(t_asm *bler, char *text, char *line)
{
    printf("%s\n", text);
    if (line)
        printf("%s\n", line);
    exit(0);
}

