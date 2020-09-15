#ifndef ASSEMBLER_ASM_H
#define ASSEMBLER_ASM_H

#include <op.h>
#include "stdio.h" // remove it
#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"
#include "../libft/ft_printf/printf.h"

# define TRUE 1
# define FALSE 0

# define ERROR_LINE "ERROR: This line have not valid parameters.\n"
# define ERROR_ARGS "ERROR: This line have not valid arguments.\n"
# define ERROR_ALLOCATE "ERROR: Didn't allocate memory.\n"
# define ERROR_ARGS_TYPE "ERROR: Some argument have wrong type.\n"
# define ERROR_TYPE_SIZE "ERROR: Type of arguments is invalid.\n"
# define ERROR_SEVERAL_COMMAS "ERROR:  Line have several commas between arguments.\n"
# define ERROR_WRONG_COUNT_ARGS "ERROR: Operation have wrong count of arguments.\n"
# define ERROR_TYPE_OF_OPER "ERROR: Operation have wrong type in arguments.\n"
# define ERROR_LBL_NAME "ERROR: This string have wrong label name:\n"
# define ERROR_FILE_EXTENSION "ERROR: File have not valid extension or this file not exist.\nPlease, check it.\n"
# define ERROR_CONTANT "ERROR: File have not valid data.\nPlease check it.\n"
# define ERROR_WOKS_NM_CM "ERROR: Name or comment have wrong with woks.\n"
# define ERROR_NAME_LEN "ERROR: Name's length is long.\n"
# define ERROR_COMM_LEN "ERROR: Comment's length is long.\n"
# define ERROR_NOT_FOUND_NM_CM "ERROR: Name or comment not fount.\n"
# define ERROR_UNKNOWN_TEXT "ERROR: This line is unknown to program:\n"
# define ERROR_END_LINE "ERROR: wrong end of file.\nPlease check content of file.\n"
# define ERROR_CHMP_SIZE "ERROR: The champs size is exceeded.\n"
# define ERROR_LBLS_NOT "ERROR: Champ file have lable which not exist.\n"
# define ERROR_DOUBLE_LBL "ERROR: file have double lable name.\nPlease, fix files labels.\n"
# define MAX_UINT 4294967295
# define NEARLY_MAX_UINT 429496729
# define SIZE_T_IND 2
# define SIZE_T_REG 1

/*
 * op_code - 1 because g_op_tab starting with 0.
 */

# define CHECK_TYPE(op_code, num, type) g_op_tab[op_code - 1].args[num].arg[(int)type]
# define CHECK_ARGS_COUNT(op_code) g_op_tab[op_code - 1].args_num

typedef struct      s_array
{
	char            arg[3];
}                   t_array;

/*
 * Recording name of operation;
 * Recording count of agruments;
 * args keeping - keeping arguments which indicate to type of argmunet.
 * cod_t_args -
 * lbl_size keeping size for translation to byte-code.
 */

typedef struct      s_op_list
{
	char            *name;
	char            op_code;
	char            lbl_size;
	int             args_num;
	t_array         args[3];
	int        	    cod_t_args; //что это? это <<Код типов аргументов>>
}                   t_op_list;

/*
 * In args - all arguments.
 * In arg - is code of types of arguments.
 * In args c
 */
//# define CHECK_TYPE(op_code, num, type) g_op_tab[op_code].args[num].arg[(int)type]
/*
 * Как происходит поиск через константную переменную?
 * Идёт обращение к коду операции - далее какой по счету этот аргумент? Обращается к номеру аргумента - если там 1, то отлично.
 * Т.е. все нули это отсуствие аргументов, а все единицы возвращают TRUE, что значит - аргумент действительно должен быть.
 */

static t_op_list  g_op_tab[16] = {
		{"live", 1, 4, 1, {{{FALSE, TRUE, FALSE}}, {{FALSE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 0},
		{"ld", 2, 4,  2, {{{FALSE, TRUE, TRUE}}, {{TRUE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 1},
		{"st", 3, 4, 2, {{{TRUE, FALSE, FALSE}}, {{TRUE, FALSE, TRUE}}, {{FALSE, FALSE, FALSE}}}, 1},
		{"add", 4, 4, 3, {{{TRUE, FALSE, FALSE}}, {{TRUE, FALSE, FALSE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"sub", 5, 4, 3, {{{TRUE, FALSE, FALSE}}, {{TRUE, FALSE, FALSE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"and", 6, 4, 3, {{{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, TRUE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"or", 7, 4,3, {{{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, TRUE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"xor", 8, 4, 3, {{{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, TRUE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"zjmp", 9, 2, 1, {{{FALSE, TRUE, FALSE}}, {{FALSE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 0},
		{"ldi", 10, 2, 3, {{{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, FALSE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"sti", 11, 2, 3, {{{TRUE, FALSE, FALSE}}, {{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, FALSE}}}, 1},
		{"fork", 12, 2, 1, {{{FALSE, TRUE, FALSE}}, {{FALSE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 0},
		{"lld", 13, 4, 2, {{{FALSE, TRUE, TRUE}}, {{TRUE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 1},
		{"lldi", 14, 2, 3, {{{TRUE, TRUE, TRUE}}, {{TRUE, TRUE, FALSE}}, {{TRUE, FALSE, FALSE}}}, 1},
		{"lfork", 15, 2, 1, {{{FALSE, TRUE, FALSE}}, {{FALSE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 0},
		{"aff", 16, 4, 1, {{{TRUE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}, {{FALSE, FALSE, FALSE}}}, 1}
};

enum {
	NUM_VAL = 1,
	STRING_VAL = 2
};

/*
 * str_val и num_val запоминают сами значения исходя из типа.
 * Детектор указывает - это строка или цифра (STRING_VAL или NUM_VAL);
 * args_size размер аргумента;
 * type - сохраняет в виде цифр тип аргумент - T_REG, T_DIR, T_IND;
 */

typedef  struct     s_arg
{
	int             detector;
	char            *str_val;
    long long       num_val;
    int             args_size;
    int             type;
    struct s_arg    *next;
}                   t_argument;

typedef struct          s_lbls
{
    char                *str;
    int                 strlen;
}                       t_lbls;

typedef  struct         s_operation
{
    char                *name;
    int 				addr;
    int 				code_type_arg; // FIXME DALER
    int                 op_code; // у него установлено char.
    int                 op_size; // FIXME DALER здесь храним размер операции
    t_lbls              *lbl;
    t_argument          *args;
    struct s_operation  *next;
}                       t_operation;

/*
 *
 */

typedef struct          s_rec
{
	char                *final_code;
	int 				cur;
	int					file_size;
	int 				file_fd;
}                       t_rec;

typedef  struct     s_asm
{
    int             fd;
    char            *files_name;
    char            *name;
    char            *comment;
    char            *line;
	t_operation     *oper;
    int             sym;
    int 			exec_code_size; // TODO DALER ОБЩИЙ РАЗМЕР КОДА
    int             line_len;
	t_rec			record;
}                   t_asm;

/*
 * Functions
 */
void                parser(t_asm *bler);
void                parse_name_comm(t_asm *bler);\
void                parse_instructions(t_asm *bler);
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

/*
 * Function of parsing arguments;
 */

void                set_args(t_asm *bler, t_operation *oper, char **args);
void                *parse_data(char *arg, int arg_type, int *detector);
void                join_argument(t_operation *oper, void *arg, int type, int detector);
void                check_commas(t_asm *bler, char **args);
void                check_arg_count_type(t_asm *bler, t_operation *oper);

/*
 * Functions of translator
 */

void		recorder(t_asm *bler);
void		get_exec_code_size(t_asm *bler);
void 		rec_init(t_asm *bler);
void 		test(t_asm *bler);
int			get_t_dir_val(t_argument *arg, t_asm *bler, t_operation *oper);
char		create_code_type_arg(t_operation *oper);
void		bytecode_conversion(t_rec *rec,int data ,int size);

/*
 * Утилиты
 */
void                print_operation(t_asm *bler, t_operation *opera);
void                clear_bler(t_asm *bler);
#endif
