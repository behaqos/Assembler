#include "asm.h"

/*
 * Пропуск комментариев решетки и точки с запятой
 * Далее смотрим - есть ли строка и не встретилась ли ещё двоеточие.
 * Если нет, то проверяем на соответствие обычным буквам. Если у строки все буквы
 * в соответствии с требованиями к метке], то проверяем сразу после окончания букв
 * есть ли двоеточие, которое всегда должно быть после метки. Если есть, значит
 * мы нашли метку.
 */

int             check_label(t_asm *bler)
{
    int         i;

    i = 0;
    pass_voids(bler);
    while (bler->line[i] && bler->line[i] != ':')
    {
        if (!ft_strchr(LABEL_CHARS, bler->line[i]))
	        return (0);
        i++;
    }
    if (i > 0 && bler->line[i] == ':')
        return (1);
    else
	    return (0);
}

void            add_lbl_list(t_asm *bler, char *str)
{
    t_lbls      *tmp;
    t_lbls      *new;

    tmp = bler->oper->lbl;
    while (tmp && tmp->next)
        tmp = tmp->next;
    if (!(new = (t_lbls *)ft_memalloc(sizeof(t_lbls))))
        return ;
    new->str = ft_strdup(str);
    new->strlen = ft_strlen(str); // TODO если нигде не используется, то удаляем.
    if (bler->oper->lbl == NULL)
        bler->oper->lbl = new;
    else
        tmp->next = new;
}

void            add_new_lbl(t_asm *bler, int *lbl_end)
{
    int         x;
    char        *res;

    x = *lbl_end;
    while (bler->line[*lbl_end] && bler->line[*lbl_end] != ':')
        (*lbl_end)++;
    res = ft_strsub(bler->line, x, *lbl_end);
    add_lbl_list(bler, res);
    free(res);
    (*lbl_end)++;
}

/*
 * i - это итератор, который читает строку.
 */

void            add_lbls(t_asm *bler, t_operation *oper)
{
    char        *str;

    while (bler->line)
    {
        pass_comments(bler->line);
        pass_voids(bler);
        if (check_label(bler))
            add_new_lbl(bler, &bler->sym);
        pass_voids(bler);
        if (bler->line[bler->sym] != '\0')
            return ;
        free(bler->line); // применил вместо ft_strdel
        if (get_next_line(bler->fd, &bler->line) > 0 && bler->line)
            bler->line_len = ft_strlen(bler->line);
    }
}

