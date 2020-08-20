#include "asm.h"

void                print_operation(t_asm *bler, t_operation *opera)
{
	t_operation     *oper;
	int             i = 1;
	oper = opera;
	t_argument      *args;

	args = oper->args;
	printf("\nString: %s\n\n", bler->line);
	printf("Operation name: %s\nLabel: %s\n\n", oper->name, oper->lbl->str);
	while (args)
	{
		printf("ARG #%d ", i);
		if (args->detector == 1)
			printf("Value: %d\n", args->num_val);
		else
			printf("Value: %s\n", args->str_val);
		//TYPES
		if (args->type == T_REG)
			printf("                Type:%s", "T_REG");
		else if (args->type == T_DIR)
			printf("                Type:%s", "T_DIR");
		else if (args->type == T_IND)
			printf("                Type:%s", "T_IND");
		args = args->next;
		i++;
		printf("\n");
	}
	exit(0);
}