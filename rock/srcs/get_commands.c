/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkiselev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:39:55 by tkiselev          #+#    #+#             */
/*   Updated: 2018/07/16 16:39:57 by tkiselev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * typedef struct			s_command
{
	char				*name; - имя операции
	char				opcode; - код операции
	unsigned int		bytes; - кол-во байтов
	unsigned int		bb; - байты до
	unsigned char		codage; ???
	t_list				*labels; - метка
	t_arg				*args; - аргументы
	struct s_command	*next; - все операции записываются в листы.
}						t_command;
 */

/*
 * Идём к концу списка и выделяем память под новый лист.
 * Обнуляем все переменные.
 * Если head был пуст, то записываем эту операцию как первую.
 * Возвращаем операцию.
 */
t_command			*push_new_command(t_command **head)
{
	t_command		*new;
	t_command		*tmp;

	tmp = *head;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!(new = (t_command*)malloc(sizeof(t_command))))
		ft_error(ERR_MALLOC);
	new->next = NULL;
	new->args = NULL;
	new->labels = NULL;
	new->opcode = 0;
	new->bb = 0;
	new->bytes = 0;
	new->codage = 0;
	new->name = NULL;
	if (!*head)
		*head = new;
	else
		tmp->next = new;
	return (new);
}

static	void		get_command(t_asm *asmb, t_command *new, int *j)
{
	char			*s;
	int				tmp;

	s = asmb->line;
	tmp = *j;
	while (s[*j] && s[*j] != '-' && s[*j] != '%' && s[*j] != ' ' &&
		s[*j] != '\t' && (s[*j] < '0' || s[*j] > '9'))
		(*j)++;
	if (s[*j] != ' ' && s[*j] != '\t' && s[*j] != '%' && s[*j] != '-')
		ft_error(ERR_STRING);
	new->name = my_strsub(s, tmp, *j);
	new->opcode = index_of(new->name, ft_strlen(new->name)) + 1;
	(new->opcode == -1) ? ft_error(ERR_COMMAND_NAME) : 0;
}

/*
** LCA means Label + Command + Arguments
*/

static	void		get_lca(t_asm *asmb)
{
	t_command		*new;
	int				j;

	j = 0;
	/* Сначала проверяет есть ли метка ИЛИ команда в линии.
	  Если один из них нашёлся, то
	  push_new_command - выделяет память под операцию.
	  get_labels - парсит в переменную new.
	*/
	if (str_has(asmb->line, LABEL) || str_has(asmb->line, COMMAND))
	{
		new = push_new_command(&asmb->command);
		get_labels(asmb, new, &j);
		if (str_has(asmb->line, COMMAND))
		{
			get_command(asmb, new, &j);
			get_arguments(asmb, new, &j);
		}
	}
	if (asmb->line)
		(!check_line(asmb->line + j)) ? ft_error(ERR_STRING) : 0;
}

//GNL каждый раз вызывается, весь текст никуда не записывается.

void				get_commands(t_asm *asmb)
{
	while (get_next_line(asmb->fd, &asmb->line) > 0)
	{
	    // записывает длину текста
		asmb->last_line_size = ft_strlen(asmb->line);
		//пропускает все строки с комментами
		comment_delete(asmb->line);

		get_lca(asmb);
		if (asmb->line)
			ft_strdel(&asmb->line);
	}
	(!check_last_line(asmb)) ? ft_error(ERR_EOF) : 0;
}
