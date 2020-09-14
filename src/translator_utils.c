#include "asm.h"

void	get_exec_code_size(t_asm *bler)
{
    int len;
    t_operation *oper;

    len = 0;
    oper = bler->oper;
    while (oper)
    {
        len += oper->op_size;
        oper->addr = len - oper->op_size; // задаем адрес
        oper = oper->next;
    }
    if (len > CHAMP_MAX_SIZE)
        error_printf(bler, ERROR_CHMP_SIZE, NULL);
    bler->exec_code_size = len;
}

void 	rec_init(t_asm *bler)
{
    bler->record.file_fd = open(
            ft_strjoin(bler->files_name, ".cor"),
            O_WRONLY | O_TRUNC | O_CREAT, 0644);
    bler->record.cur = 0;
    bler->record.file_size =
            PROG_NAME_LENGTH + COMMENT_LENGTH + (4 * 4) + bler->exec_code_size;
    if ((bler->record.final_code = ft_strnew(bler->record.file_size)) == NULL)
        error_printf(bler, "ERROR MALLOC\n", NULL); // FIXME HELP AKI BEKHRUZ
}

