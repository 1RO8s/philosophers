.DEFAULT_GOAL := all

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -pthread
SANITIZE_THREAD := -fsanitize=thread
SANITIZE_ADDRESS := -fsanitize=address
DEBUG_FLAGS := -g3

NAME	:= philo
SRCS_DIR	:= srcs
SRC_FILES := main.c print.c monitor.c timer.c config.c forks.c philo.c utils.c check.c debug.c
SRCS := $(addprefix $(SRCS_DIR)/, $(SRC_FILES))
OBJS	:= $(SRCS:.c=.o)
INC := -Iincludes

%.o: %.c
	$(CC) ${CFLAGS} ${INC} -c $< -o $@

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $(NAME)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)
	# lldb ./$(NAME)

tsan: CFLAGS += $(SANITIZE_THREAD)
tsan: re

asan: CFLAGS += $(SANITIZE_ADDRESS)
asan: re

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re debug tsan asan
