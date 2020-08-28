#include "asm.h"

void                print_operation(t_asm *bler, t_operation *opera)
{
	t_operation     *oper;
	int             i = 1;
	t_argument      *args;

	oper = opera;
	args = opera->args;
	printf("\nString: %s\n\n", bler->line);
	printf("Operation name: %s\n", opera->name);
	if (oper->lbl)
		printf("Label name: %s\n", oper->lbl->str);
	else
		printf("\n");
	while (args)
	{
		printf("ARG #%d ", i);
		if (args->detector == NUM_VAL)
			printf("Value: %d\n", args->num_val);
		else if (args->detector == STRING_VAL)
			printf("Value: %s\n", args->str_val);
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
}