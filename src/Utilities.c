/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:22:18 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:55:50 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void                print_operation(t_asm *bler, t_operation *opera)
{
	t_operation     *oper;
	int             i = 1;
	t_argument      *args;

	oper = opera;
	args = opera->args;
	ft_printf("\nString: %s\n\n", bler->line);
	ft_printf("Operation name: %s\n", opera->name);
	ft_printf("Operation size: %d byte.\n", opera->op_size);
	ft_printf("Code type size: %d byte.\n", opera->code_type_arg);
	ft_printf("OPCODE: %.2x.\n", opera->op_code);
	ft_printf("addr: %d\n", opera->addr);
	if (oper->lbl)
		ft_printf("Label name: %s\n", oper->lbl->str);
	else
		ft_printf("\n");
	while (args)
	{
		ft_printf("ARG #%d ", i);
		if (args->detector == NUM_VAL)
			ft_printf("Value: %lld\n", args->num_val);
		else if (args->detector == STRING_VAL)
			ft_printf("Value: %s\n", args->str_val);
		if (args->type == T_REG)
			ft_printf("                Type:%s", "T_REG");
		else if (args->type == T_DIR)
			ft_printf("                Type:%s", "T_DIR");
		else if (args->type == T_IND)
			ft_printf("                Type:%s", "T_IND");
		ft_printf("\nARG SIZE: %d byte\n", args->args_size);
		args = args->next;
		i++;
		ft_printf("\n");
	}
	ft_printf("\n--------------------------------------\n");

}


//#include <stdio.h>
//#include <string.h>
//
//# define COREWAR_EXEC_MAGIC		0x00ea83f3
//
//void	bytecode_conversion(char *data, int size)
//{
//	int			i;
//	//char *str[;
//
//	i = 0;
//	while (i < size)
//	{
//		// printf("%.2hhx\n", (char)((data >> ((size - i - 1) * 8)) & 0xFF));
//		printf("%.2hhx\n", data[size - i - 1]);
//		str[i] = data[size - i - 1];
//		i++;
//	}
//}

// void test(int a)
// {
//   int n = 32;
//   printf("0b");
//   while (n > 0)
//   {
//     n--;
//     printf("%d", (a >> n) & 0x1);
//   }
//   printf("\n");
// }

//int a;      // 1000 1003  == 4
//char b[4]; // 1004 1007  == 4
//char *p;
//
//p = (char *)&a;
//
//
//
//int main(void) {
//	int a = COREWAR_EXEC_MAGIC;
//	bytecode_conversion((char *)&a, sizeof(a));
//
//	// test(-2147483648);
//	// test(2147483647);
//	return 0;
//}

/*

bytecode_conversion(bler->write.final, COREWAR_EXEC_MAGIC, 4, cur);


int			bytecode_conversion(char *src, int data, int size, int cur)
{
	int			i;

	i = 0;
	while (i < size)
	{
    src[cur + i] = (char)((data >> ((size - i - 1) * 8)) & 0xFF);
		i++;
	}
	return (size);
}

*/


//printf("size final_code: %lu\n", sizeof(rec->final_code));
//printf("file_size: %d\n", rec->file_size);
//printf("cur: %d\n", rec->cur);
//printf("Exec code size:%d\n\n", bler->exec_code_size);
