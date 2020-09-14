#include "asm.h"

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

char			create_code_type_arg(t_operation *oper)
{
    char		res;
    int			i;
    t_argument	*args;

    i = 0;
    res = 0;
    args = oper->args;
    while (i < 3 || args)
    {
        if (args)
        {
            if (args->type == T_REG)
                res |= 1;
            else if (args->type == T_DIR)
                res |= 2;
            else if (args->type == T_IND)
                res |= 3;
            args = args->next;
        }
        res = res << 2;
        i++;
    }
    return (res);
}

int		get_t_dir_val(t_argument *arg, t_asm *bler, t_operation *oper)
{
    // Нужно обрезать число
    int res;
    res = 0;
    t_operation *new;

    new = bler->oper;
    if (arg->detector == NUM_VAL)
    {
        return (arg->num_val);
    }
    else if (arg->detector == STRING_VAL)
    {
        while (new)
        {
            if (new->lbl && !ft_strncmp(arg->str_val,
                                        new->lbl->str, ft_strlen(arg->str_val)))
                return (new->addr - oper->addr);
            new = new->next;
        }
        error_printf(bler,ERROR_LBLS_NOT, NULL);
    }
    return (res);
}

void 	test(t_asm *bler)
{
    t_operation *oper;
    t_rec *rec;
    t_argument *args;

    rec = &bler->record;
    oper = bler->oper;
    while (oper)
    {
        args = oper->args;
        if (oper->name != NULL)
        {
            bytecode_conversion(rec, oper->op_code, 1);
            if (oper->code_type_arg)
                bytecode_conversion(rec, create_code_type_arg(oper), oper->code_type_arg);
            while(args)
            {
                if (args->type == T_REG || args->type == T_IND)
                    bytecode_conversion(rec, (int)args->num_val, args->args_size);
                else if (args->type == T_DIR)
                    bytecode_conversion(rec, get_t_dir_val(args, bler, oper), args->args_size);
                args = args->next;
            }
        }
        oper = oper->next;
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
    ft_putstr("Writing output program to ");
    ft_putstr(bler->files_name);
    ft_putstr(".cor file\n");
    test(bler);
   //FIXME это нужно или убрать? //write(bler->record.file_fd, rec->final_code, rec->file_size);
    write(open("111111.cor", O_WRONLY | O_TRUNC | O_CREAT, 0644), rec->final_code, rec->file_size);
}