

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

# define MAX_UINT 4294967295
# define NEARLY_MAX_UINT 429496729
# define WRONG_TYPE -1

typedef enum {
	NUM_VAL = 1,
	STRING_VAL
};

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
    char            *args_size;
    int            type;
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
void                parse_commands(t_asm *bler);
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
