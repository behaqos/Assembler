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
 * Здесь берётся конец настоящей строки + терминированный нуль.
 * Очищается через ft_bzero
 * Он заново парсит строку метки перед пустой строкой.
 * Если в ней находит операцию либо лейбл, то отлично. Иначе проверяет есть ли там \n.
 * Если его нет, тогда выводи ошибку.
 */

char		check_last_line(t_asm *asmb)
{
	char	s[asmb->last_line_size + 1];
	int		i;

	i = 0;
	ft_bzero(s, asmb->last_line_size + 1); // Обнуляет всю строку без очистки.
	lseek(asmb->fd, -asmb->last_line_size, SEEK_CUR); // TODO Что здесь происходит?
	read(asmb->fd, &s, asmb->last_line_size); // Читает fd-шник в s на прежний размер байтов
	if (!str_has(s, LABEL) && !str_has(s, COMMAND)) // Если есть метка или операция, то возв-ет единицу.
		return (1);
	else
	{
		i = ft_strlen(s) - 1; //Проверяет есть ли там перенос строки. Если нет, то возвращает ошибку.
		if (s[i] == '\n')
			return (1);
	}
	return (0);
}

char		str_has(char *str, char flag)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (flag == LABEL)
	{//пока есть строка и нет двоеточия
		while (str[i] && str[i] != ':')//двигаемся по строке и проверяем все ли символы там подходящие.
		    //Если один из символов строки str нет в строке алфавита LABEL_CHARS, то выкидывает.
			if (!ft_strchr(LABEL_CHARS, str[i++]))
				return (0);
			//А когда все символы совпали, а дальше идёт двоеточие, то это точно метка, возвращаем сигнал, что это метка.
		return ((i > 0 && str[i] == ':') ? 1 : 0);
	}
    /*
     * Если флаг команды, то проверяем сначала есть ли в строке метка.
     * Если есть метка в строке, то ищем двоеточие. А если двоеточия нет, то ничего.
     * Идём по строке и пропускаем пробелы и табуляции.
     * Если ещё в строке остались буквы и есть буквы, пропускаем все буквы.
     * index_of ищет операцию. Если не нашёл операцию, то возвращает -1
     * А если нашёл операцию, то возв-ет единицу.
     */
	else if (flag == COMMAND)
	{
		(str_has(str, LABEL)) ? (str = ft_strchr(str, ':') + 1) : 0;
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		while (str[i] && ft_isalpha(str[i]))
			i++;
		return ((i > 0 && index_of(str, i) != -1) ? 1 : 0);
	}
	return (0);
}

/*
 * Выделяем память под s. Очищаем строку.
 * Записываем туда название операции.
 */

char		*my_strsub(char *src, int start, int end)
{
	char	*s;
	int		i;
	int		size;

	size = end - start;
	if (!(s = (char *)malloc(size + 1)))
		ft_error(ERR_MALLOC);
	ft_bzero(s, size + 1);
	i = 0;
	while (i < size)
	{
		s[i] = src[start + i];
		i++;
	}
	return (s);
}

void		check_argvs(t_asm *asmb, char **av, int ac)
{
	while (--ac > 0)
	{   // проверяет флаги и записывает имя файла.
		if (ft_strequ(av[ac], "-a"))
			asmb->flag_a = 1;
		else if (ft_strequ(av[ac], "-b"))
			asmb->flag_b = 1;
		else if (!asmb->file_name)
			asmb->file_name = ft_strdup(av[ac]);
	}
}

void		norm_file_name(char **file_name)
{
	char	*tmp;

	if (*file_name)
	{
		if (ft_strrchr(*file_name, '/'))
		{
			tmp = ft_strdup(ft_strrchr(*file_name, '/') + 1);
			ft_strdel(file_name);
			*file_name = tmp;
		}
	}
}
