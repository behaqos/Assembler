/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 03:09:49 by dpenney           #+#    #+#             */
/*   Updated: 2019/10/17 13:56:21 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	rec_line(int fd, char **line, char **rem)
{
	int		i;
	char	*t_ptr2;

	i = 0;
	while (rem[fd][i] != '\n' && rem[fd][i] != '\0')
		i++;
	if (rem[fd][i] == '\n')
	{
		*line = ft_strsub(rem[fd], 0, i);
		t_ptr2 = ft_strdup(rem[fd] + i + 1);
		free(rem[fd]);
		rem[fd] = t_ptr2;
		if (rem[fd][0] == '\0')
			ft_strdel(&rem[fd]);
	}
	else if (rem[fd][i] == '\0')
	{
		*line = ft_strdup(rem[fd]);
		ft_strdel(&rem[fd]);
	}
	return (1);
}

static int	give_line(int fd, char **line, char **rem, char *buff)
{
	char		*t_ptr;
	int			rsr;

	rsr = 0;
	while ((rsr = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[rsr] = '\0';
		if (rem[fd] == NULL)
			rem[fd] = ft_strnew(1);
		t_ptr = ft_strjoin(rem[fd], buff);
		free(rem[fd]);
		rem[fd] = t_ptr;
		if (ft_strchr(buff, '\n'))
			break ;
	}
	free(buff);
	if (rsr < 0)
		return (-1);
	if (rsr == 0 && (rem[fd] == NULL || rem[fd][0] == '\0'))
		return (0);
	return (rec_line(fd, line, rem));
}

int			get_next_line(int fd, char **line)
{
	char		*buff;
	static char	*rem[FD_MAX];

	if (fd < 0 || !line || fd > FD_MAX || \
								BUFF_SIZE <= 0)
		return (-1);
	if (!(buff = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (-1);
	return (give_line(fd, line, rem, buff));
}