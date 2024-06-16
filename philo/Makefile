.DEFAULT_GOAL := all

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -pthread
SANITIZE_THREAD := -fsanitize=thread
SANITIZE_ADDRESS := -fsanitize=address
DEBUG_FLAGS := -g3
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

debug: CFLAGS += $(DEBUG_FLAGS)
debug: fclean $(NAME)
	# lldb ./$(NAME)

tsan: CFLAGS += $(SANITIZE_THREAD)
tsan: fclean $(NAME)

asan: CFLAGS += $(SANITIZE_ADDRESS)
asan: fclean $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re tsan asan debug
