//
// Created by Duram Penney on 8/11/20.
//

#include <fcntl.h>
#include "../includes/asm.h"

void            check_file(t_asm *bler, char *str)
{
    bler->files_name = ft_strdup(str);
    if (!bler->files_name)
        error_printf(bler, "Error: Can't to read name of the file\n", NULL);
    bler->fd = open(bler->files_name, O_RDONLY);
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

// __________________________________________

void	get_exec_code_size(t_asm *bler)
{
	int len;
	t_operation *oper;

	len = 0;
	oper = bler->oper;
	while (oper)
	{
		len += oper->op_size;
		oper = oper->next;
	}
	if (len > CHAMP_MAX_SIZE)
		error_printf(bler, "Размер чемпиона больше допустимого.\n", NULL); //FIXME DALER CHEK THIS ERROR
	bler->exec_code_size = len;
}

//

void 	rec_init(t_asm *bler)
{
	bler->record.cur = 0;
	bler->record.file_size =
		PROG_NAME_LENGTH + COMMENT_LENGTH + (4 * 4) + bler->exec_code_size;
	if ((bler->record.final_code = ft_strnew(bler->record.file_size)) == NULL)
		error_printf(bler, "ERROR MALLOC\n", NULL); // FIXME HELP AKI BEKHRUZ
}

void		bytecode_conversion(t_rec *rec,int data ,int size)
{
	int	i;

	i = 0;
	rec->cur += size;
	while (size > i)
	{
		rec->final_code[rec->cur - i - 1] = (char)((data >> (i * 8)) & 0xFF);
		i++;
	}
}

void	recorder(t_asm *bler)
{
	t_rec *rec;
	int fd;

	get_exec_code_size(bler);
	rec_init(bler);
	rec = &bler->record;
	bytecode_conversion(rec, COREWAR_EXEC_MAGIC, 4);
	ft_memcpy(&rec->final_code[rec->cur], bler->name, ft_strlen(bler->name));
	rec->cur += PROG_NAME_LENGTH + 4;
	bytecode_conversion(rec, bler->exec_code_size, 4);
	ft_memcpy(&rec->final_code[rec->cur], bler->comment, ft_strlen(bler->comment));
	rec->cur += COMMENT_LENGTH + 4;

	fd = open("111111.cor", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	write(fd, rec->final_code, rec->file_size);
}

int             main(int argc, char **argv)
{
    t_asm       bler;

    ft_bzero(&bler, sizeof(bler)); // FIXME Так делать правильно
    if (argc == 1 || argc > 2)
        error_printf(&bler, "Error\n", NULL);
    check_extension(argv[1]);
    check_file(&bler, argv[1]);
    parser(&bler);
	recorder(&bler);
	printf("%.8x",0x00ea83f3 >> 4);
    exit(0);
}