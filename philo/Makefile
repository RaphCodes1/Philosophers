NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRCS = main.c\
	   init.c\
	   lock_unlock.c\
	   utils.c\
	   status_utils.c\
	   mutex_and_thread.c\
	   monitor.c\
	   sync.c\
	   status.c\
		helpers/ft_atoi.c\
		helpers/ft_strlen.c\
		helpers/ft_putstr_fd.c\
		helpers/ft_atol.c\

OBJS = $(SRCS:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -pthread

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ -pthread

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
