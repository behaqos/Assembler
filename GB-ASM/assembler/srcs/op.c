/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwillem- <jwillem-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/10/04 11:43:01 by zaz               #+#    #+#             */
/*   Updated: 2019/06/15 02:59:55 by gabshire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * В g_op_tab записываются все операции. В каком порядке?
 * Сначала имя операции, сколько аргументов принимает, какой тип аргумента принимает
 * код операции, **	cycles циклы до исполнения
**	descrip короткое описание
**	a_typecode - код типов аргументов
**	dir_size размер т-дир
*/
// TODO ЭТО ГЛОБАЛЬНАЯ ПЕРЕМЕННАЯ?
t_op	g_op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
	{{0}, 0, {0}, 0, 0, {0}, 0, 0}
};

/*
 * Если операция нашлась (проверяется по присутствию имени)
 * то смотрим - если нет пробела либо табуляции после имени операции, то очищаем всё и выводим ошибку.
 */

static void	operation_last(t_op *v, t_all *all, int *i)
{
	if (v->cmd[0])
	{
		{
			if (SPLIT[all->i + ft_strlen((char *)v->cmd)] != ' ' &&
				SPLIT[all->i + ft_strlen((char *)v->cmd)] != '\t')
			{
				ft_bzero(v, sizeof(v));
				i[0] = -1;
			}
		}
	}
}

/*
 * выделяет  память под t_op.
 * Присваивает i к переменной. (i хранит цифру, на которой остановился, когда читал линию).
 * Запускает цикл от 0 до 17
 * Там ищём операцию по имени.
 * И если нашли, то смотрим - после операции есть пробел либо табуляция, которая должна быть?
 * Если есть, то отлично. Если нет, то очишаем v
 */

t_op		operations(t_all *all, int *i)
{
	char		*str;
	unsigned	vars[3];
	t_op		v;

	vars[0] = all->i;
	vars[1] = 0;
	ft_bzero(&v, sizeof(v));
	while (vars[1] < 17)
	{
		all->i = vars[0] - 1;
		vars[2] = 0;
		//записывает имя первой операции в str
		str = (char *)g_op_tab[vars[1]].cmd;
		// сравнивает строки по именам операции. Можно заменить с ft_strncmp.
		while (SPLIT[++all->i] && str[vars[2]] && SPLIT[all->i] == str[vars[2]])
			++vars[2];
		if (!str[vars[2]] && vars[1] < 16)
		{
			v = g_op_tab[vars[1]];
			(*i) = vars[1];
		}
		++vars[1];
	}
	operation_last(&v, all, i);
	return (v);
}