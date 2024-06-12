.DEFAULT_GOAL := all

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror
NAME	:= philo
SRCS	:= srcs/main.c srcs/print.c
OBJS	:= $(SRCS:.c=.o)
SRCS_DIR	:= srcs
INC := -Iincludes

%.o: %.c
	$(CC) ${CFLAGS} ${INC} -c $< -o $@

all:	$(NAME)
$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re
