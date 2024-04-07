.PHONY: all clean fclean re
.DEFAULT_GOAL := all

LIBFT_DIR	:= libft
LIBFT_NAME := libft/libft.a

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror
NAME	:= philo
SRCS	:= srcs/main.c
OBJS	:= $(SRCS:.c=.o)
SRCS_DIR	:= srcs
INC_DIR	:= includes
INC := -I$(INC_DIR) -I$(LIBFT_DIR)

%.o: %.c
	$(CC) ${CFLAGS} ${INC} -c $< -o $@

all:	$(NAME)
$(NAME):	$(OBJS)
  make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(INC) $(LIBFT_NAME) $(OBJS) -o $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIBFT_NAME) $(NAME)

re:		fclean all

.PHONY: all clean fclean re