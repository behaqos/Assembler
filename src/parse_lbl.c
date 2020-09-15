/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lbl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:51 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:56 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        return (TRUE);
    else
	    return (FALSE);
}

/*
 * At first keeping current lbl in tmp.
 * Allocating memory for new laberl (new_lbl)
 * Saving string to new_lbl.
 * Looking for last label of operation. If last label exist - keeping to pointer of next
 * new label. If don't exist - keeping pointer like first label.
 */

void            add_lbl_list(t_operation *oper, char *str)
{
    t_lbls      *new_lbl;

    if (!(new_lbl = (t_lbls *)ft_memalloc(sizeof(t_lbls))))
        return ;
	new_lbl->str = ft_strdup(str);
	new_lbl->strlen = ft_strlen(str);
    oper->lbl = new_lbl;
}

/*
 * At first checking string and finding string which completing with ':'
 * In lbl_end we are keeping position of end of the string.
 * After it copy string with start and end by lbl_end.
 * When we find our string - sending to addition to operation list.
 * lbl_end incrementing than to parse in next
 */

void            add_new_lbl(t_asm *bler, int *lbl_end, t_operation *oper)
{
    int         start;
    char        *res;

	start = *lbl_end;
    while (bler->line[*lbl_end] && bler->line[*lbl_end] != ':')
        (*lbl_end)++;
    res = ft_strsub(bler->line, start, *lbl_end);
	(*lbl_end)++;
    add_lbl_list(oper, res);
    free(res);
}



/*
 * Если есть строка, то очищаем её.
 * Далее перемещаем fd дескриптор назад на кол-во символов (байтов).
 * Считываем заново.
 */

/*
 * Here we are missing all comments and voids like tabs and spaces.
 * Aftre it checking label. If label exist - we are adding this label
 * like new list to operation list. And after it checking next line unitl
 * won't find new string with operations or lablers.
 */

void            add_lbls(t_asm *bler, t_operation *oper)
{
	t_operation *new_oper;

	new_oper = oper;
    while (bler->line)
    {
        pass_comments(bler->line);
        pass_voids(bler);
        if (check_label(bler))
        {
        	new_oper->lbl ? new_oper = init_op_list(bler) : 0;
	        add_new_lbl(bler, &bler->sym, new_oper);
        }
        pass_voids(bler);
        if (bler->line[bler->sym] != '\0')
            return ;
        bler->line ? ft_strdel(&bler->line) : 0; // применил вместо ft_strdel
        if (get_next_line(bler->fd, &bler->line) > 0 && bler->line)
        {
        	bler->line_len = ft_strlen(bler->line);
        	bler->sym = 0;
        }
    }
}

