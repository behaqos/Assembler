
NAME=asm
CC = gcc
FLAGS = -Wall -Wextra -Werror
SRC = ./src/asm.c\
        ./src/errors.c\
        ./src/join_args_to_oper.c\
        ./src/parse_args.c\
        ./src/parse_data.c\
        ./src/parse_instruments.c\
        ./src/parse_lbl.c\
        ./src/parse_name.c\
        ./src/parse_op.c\
        ./src/parser.c\
        ./src/set_args.c\
        ./src/translator.c\
        ./src/translator_utils.c\
        ./src/Utilities.c

HEADERS = includes/asm.h includes/op.h
OBJ=$(SRC:.c=.o)
LIBFT_HEADER=./libft/includes/libft.h ./libft/ft_printf/printf.h ./libft/ft_printf/apply_spec.h ./libft/ft_printf/fp_type.h ./libft/ft_printf/read_spec.h
LIBFT=./libft/libftprintf.a
INCLUDES=./includes

all: $(NAME)

%.o: %.c $(INCLUDES) $(LIBFT)
	$(CC) $(FLAGS) -o $@ -c $< -I $(INCLUDES)

$(NAME): $(LIBFT) $(OBJ) $(HEADERS) $(LIBFT_HEADER)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C ./libft

clean:
	rm -f $(OBJ)
	make clean -C ./libft

fclean: clean
	rm -f $(NAME)
	make fclean -C ./libft/

re: fclean all

.PHONY: all, clean, fclean, re
