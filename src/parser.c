#include "asm.h"

void        parser(t_asm *bler)
{
    parse_name_comm(bler); // TODO запарсить имя и комментарий
	parse_operations(bler);
	//TODO Проверить нашлись ли операции. Если нет, то вывод ошибки.
	//TODO высчитать размер программы? asmb->prog_size = compute_variables(asmb->command);
}

