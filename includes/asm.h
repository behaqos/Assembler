

#ifndef ASSEMBLER_ASM_H
#define ASSEMBLER_ASM_H

#include <op.h>
#include "stdio.h" // remove it
#include "../libft/libft.h"

# define TRUE 1
# define FALSE 0
# define ERROR_LINE "ERROR: This line have not valid parameters.\n"
# define ERROR_ARGS "ERROR: This line have not valid arguments.\n"
# define ERROR_ALLOCATE "ERROR: Didn't allocate memory.\n"
# define ERROR_ARGS_TYPE "ERROR: Some argument have wrong type.\n"
# define ERROR_TYPE_SIZE "ERROR: Type of arguments is invalid\n"
# define MAX_UINT 4294967295
# define NEARLY_MAX_UINT 429496729
# define WRONG_TYPE -1
# define SIZE_T_IND 2
# define SIZE_T_DIR 2
# define SIZE_T_REG 1

typedef struct      s_array
{
	char            arg[3];
}                   t_array;

typedef struct      s_op_list
{
	char            *name;
	char            args_num;
	t_array         args[3];
	char            op_code;
	char            codage;
	char            lbl_size;
}                   t_op_list;

static t_op_list  g_op_tab[16] = {
		{"live", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 1, 0, 4},
		{"ld", 2, {{{0, 1, 1}}, {{1, 0, 0}}, {{0, 0, 0}}}, 2, 1, 4},
		{"st", 2, {{{1, 0, 0}}, {{1, 0, 1}}, {{0, 0, 0}}}, 3, 1, 4},
		{"add", 3, {{{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}}, 4, 1, 4},
		{"sub", 3, {{{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}}, 5, 1, 4},
		{"and", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 6, 1, 4},
		{"or", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 7, 1, 4},
		{"xor", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 8, 1, 4},
		{"zjmp", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 9, 0, 2},
		{"ldi", 3, {{{1, 1, 1}}, {{1, 1, 0}}, {{1, 0, 0}}}, 10, 1, 2},
		{"sti", 3, {{{1, 0, 0}}, {{1, 1, 1}}, {{1, 1, 0}}}, 11, 1, 2},
		{"fork", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 12, 0, 2},
		{"lld", 2, {{{0, 1, 1}}, {{1, 0, 0}}, {{0, 0, 0}}}, 13, 1, 4},
		{"lldi", 3, {{{1, 1, 1}}, {{1, 1, 0}}, {{1, 0, 0}}}, 14, 1, 2},
		{"lfork", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 15, 1, 2},
		{"aff", 1, {{{1, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 16, 1, 4}
};

typedef enum {
	NUM_VAL = 1,
	STRING_VAL = 2
};

typedef enum types
{
    LABEL = 1,
    COMMAND
};

/*
 * str_val и num_val запоминают сами значения исходя из типа.
 * Детектор указывает - это строка или цифра (STRING_VAL или NUM_VAL);
 * args_size размер аргумента;
 * type - сохраняет в виде цифр тип аргумент - T_REG, T_DIR, T_IND;
 */

typedef  struct     s_arg
{
    char            *str_val;
    int             num_val;
    int             detector;
    int            args_size;
    int             type;
    struct s_arg    *next;
}                   t_argument;

typedef struct          s_lbls
{
    char                *str;
    int                 strlen;
    struct s_lbls       *next;
}                       t_lbls;

typedef  struct         s_operation
{
    char                *name;
    int                 op_code; // у него установлено char.
    int                 byte;
    t_lbls              *lbl;
    t_argument          *args;
    struct s_operation  *next;
}                       t_operation;

/*
 *
 */

typedef  struct     s_asm
{
    int             fd;
    char            *files_name;
    char            *name;
    char            *comment;
    char            *line;
    char            *free_line; //TODO эта переменная понадобится, если line до конца не очищается.
    int             sym;
    int             line_len;
    int             cor_fd;
    t_operation     *oper;
}                   t_asm;

/*
 * Functions
 */
void                parser(t_asm *bler);
void                parse_name_comm(t_asm *bler);\
void                parse_operations(t_asm *bler);
t_operation         *init_op_list(t_asm *bler);
void                parse_args(t_asm *bler, t_operation *oper);

int                 check_label(t_asm *bler);
void                add_lbls(t_asm *bler, t_operation *oper);

int                 check_op(t_asm *bler);
void                add_op(t_asm *bler, t_operation *oper);
/*
 * Instruments
 */
void                pass_comments(char *str);
void                pass_voids(t_asm *bler);
void                error_printf(t_asm *bler, char *text, char *line);

#endif
