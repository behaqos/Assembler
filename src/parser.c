#include "asm.h"

//TODO разбить функции по файлам

void        parser(t_asm *bler)
{
    parse_name_comm(bler); // TODO запарсить имя и комментарий
	parse_instructions(bler);
	//TODO plavleno, убрать две строки ниже в другую функцию.
	if (!bler->oper || !bler->oper->args)
		error_printf(bler, ERROR_CONTANT, NULL);
	//TODO высчитать размер программы? asmb->prog_size = compute_variables(asmb->command);
}
