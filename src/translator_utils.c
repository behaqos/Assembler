/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translator_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:22:11 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:45:00 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * Функция которая вычисляет размер бущуего чемпиона,
 * а так же задает адрес меткам и операциям.
 */

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

/*
 * Инициализация переменных для дальнейшей работы с записью в файл.
 */

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
