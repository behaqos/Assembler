/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:31 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:52 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"

void           		clean_arguments(t_argument *args)
{
	t_argument      *tmp;

	tmp = args;
	while (tmp)
	{
		tmp->str_val ? free(tmp->str_val) : 0;
		tmp = tmp->next;
	}
}

void                clear_operations(t_operation *opers)
{
	t_operation     *tmp;

	tmp = opers;
	while (tmp)
	{
		tmp->name ? free(tmp->name) : 0;
		tmp->lbl && tmp->lbl->str ? free(tmp->lbl->str) : 0;
		tmp->lbl ? free(tmp->lbl) : 0;
		clean_arguments(tmp->args);
		tmp = tmp->next;
	}
}

void                clear_bler(t_asm *bler)
{
	if (bler->files_name != NULL)
		free(bler->files_name);
	//bler->files_name != NULL ? free(bler->files_name) : 0;
	bler->name ? free(bler->name) : 0;
	bler->comment ? free(bler->comment) : 0;
    clear_operations(bler->oper);
}

void                error_printf(t_asm *bler, char *text, char *line)
{
    text ? ft_printf("%s\n", text) : 0;
    line ? ft_printf("%s\n", line) : 0;
	if (bler)
		clear_bler(bler);
    exit(0);
}
