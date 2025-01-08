NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRCS = main.c\
		libft/ft_atoi.c\
		libft/ft_strlen.c\

OBJS = $(SRCS:.c=.o)

all:$(NAME)

#$(CFLAGS)
$(NAME): $(OBJS)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
