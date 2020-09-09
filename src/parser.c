#include "asm.h"

void        parser(t_asm *bler)
{
    parse_name_comm(bler);
	ft_printf("NAME:    %s\n", bler->name);
	ft_printf("COMMENT:    %s\n", bler->comment);
	parse_instructions(bler);
	//TODO plavleno, убрать две строки ниже в другую функцию.
	if (!bler->oper)
		error_printf(bler, ERROR_CONTANT, NULL);

	//TODO высчитать размер программы? asmb->prog_size = compute_variables(asmb->command);
}
