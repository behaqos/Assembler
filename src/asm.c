//
// Created by Duram Penney on 8/11/20.
//

#include <fcntl.h>
#include "../includes/asm.h"

void            check_file(t_asm *bler, char *str)
{
	bler->files_name = ft_strsub(str, 0, ft_strlen(str) - 2);
	if (!bler->files_name)
		error_printf(bler, "Error: Can't to read name of the file\n", NULL);
	bler->fd = open(str, O_RDONLY);
	if (bler->fd < 0)
		error_printf(bler, "Error: Can't to open this file\n", NULL);
	if (read(bler->fd, 0, 0) == -1)
		error_printf(bler, "Error: Can't to read this file\n", NULL);
}

void            check_extension(const char *str)
{
	if (ft_strcmp(&str[ft_strlen(str) - 2], ".s") == 0)
		return ;
	error_printf(NULL, ERROR_FILE_EXTENSION, NULL);
}

//TODO провести по двум чекерам в FAQ 21

int             main(int argc, char **argv)
{
    t_asm       bler;

    ft_bzero(&bler, sizeof(bler));
    if (argc == 1 || argc > 2)
        error_printf(&bler, "Error\n", NULL);
    check_extension(argv[1]);
    check_file(&bler, argv[1]);
    parser(&bler);
	recorder(&bler);
	clear_bler(&bler);
	exit(0);
}
