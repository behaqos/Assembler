#include "asm.h"



/*
 * Проверяем строку на наличие символов комментария
 * Если ещё не достигнуто конца строки, то обнуляем эту строку.
 */
void            pass_comments(char *str)
{
    int         i;

    i = 0;
    while (str[i] &&
           str[i] != ';' &&
           str[i] != '#')
        i++;
    if (str[i] != '\0')
        ft_bzero(str, ft_strlen(str));
}

void            pass_voids(t_asm *bler)
{
    while (bler->line[bler->sym] && (bler->line[bler->sym] == ' ' || bler->line[bler->sym] == '\t'))
        bler->sym++;
}

t_operation     *init_op_list(t_asm *bler)
{
    t_operation *tmp;
    t_operation *new_op;

    tmp = bler->oper;
    while (tmp && bler->oper->next)
        tmp = tmp->next;
    if (!(new_op = (t_operation *)ft_memalloc(sizeof(t_operation))))
        error_printf(bler, "Error: Program did not allocate memory\n", NULL);
    if (!bler->oper)
        bler->oper = new_op;
    else
        tmp->next = new_op;
    return (new_op);
}

/*
 * Сначала проверяем нашлись ли команда либо метка.
 * Если да, то сначала создаём переменную для хранения операций.
 * Далее добавляем метку. Если её нет, то ничего страшного, внутри проводится
 * ещё одна проверка.
 * Далее проверяем на операции.
 */

void            parse_lbl_op(t_asm *bler)
{
    t_operation *oper;

    if (check_label(bler) || check_op(bler))
    {
        oper = init_op_list(bler);
        add_lbls(bler, oper);
        add_op(bler, oper);
    }
    //TODO 	if (asmb->line)
	//TODO		(!check_line(asmb->line + j)) ? ft_error(ERR_STRING) : 0;
}

void            parse_operations(t_asm *bler)
{
    while (get_next_line(bler->fd, &bler->line) > 0)
    {
        bler->line_len = ft_strlen(bler->line);
        pass_comments(bler->line);
        parse_lbl_op(bler);
    }
    // TODO check_last_line(bler)
}
