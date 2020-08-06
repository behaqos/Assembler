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

static void	operation_last(t_op *v, t_all *all, int *i)
{
    /*
     * Проверяем через условный оператор нашлась ли операция через
     * проверку имени. Если имя есть, то дальше
     */
	if (v->cmd[0])
	{
		{
		    // складывает итератор символов, на котором остановился с длиной операции и проверяет нет ли там
		    // пробела либо табуляции.
			if (SPLIT[all->i + ft_strlen((char *)v->cmd)] != ' ' &&
				SPLIT[all->i + ft_strlen((char *)v->cmd)] != '\t')
			{
			    // Если нет, то очищает структуру и ставит значение единицы.
				ft_bzero(v, sizeof(v));
				i[0] = -1;
			}
			// Соот-но, если есть, то ничего не делает с новой операцией.
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

//Здесь мы ищем только операции.
t_op		operations(t_all *all, int *i)
{
	char		*str;
	// Вместо создания 3х переменных - создаёт просто массив;
	unsigned	vars[3];
	t_op		v;

	vars[0] = all->i;
	vars[1] = 0;
	ft_bzero(&v, sizeof(v));
    // 17, т.к. всего операций может быть 16 операций.
	while (vars[1] < 17)
	{
		all->i = vars[0] - 1;
		vars[2] = 0;
		//записывает имя первой операции из справочника всех операция в str
		str = (char *)g_op_tab[vars[1]].cmd;
		// сравнивает строки по именам операции. Можно заменить с ft_strncmp.
		// Если символы из справочника совпадают со словами из строки, то идём дальше.
		while (SPLIT[++all->i] && str[vars[2]] && SPLIT[all->i] == str[vars[2]])
			++vars[2];
		// Если букв не осталось в строке И итератор справочника ещё не закончился
		// значит строка совпала, мы нашли операцию.
		if (!str[vars[2]] && vars[1] < 16)
		{
		    //записываем все данные по найденной операции
			v = g_op_tab[vars[1]];
			// i - записываем в итератор ту позицию, в которой остановили поиск по справочнику.
			// т.е. это переменная, которая сохраняет позицию, где нашёл операцию.
			(*i) = vars[1];
		}
		++vars[1];
	}
	//Далее проверяем что в продолжении
	operation_last(&v, all, i);
	return (v);
}
