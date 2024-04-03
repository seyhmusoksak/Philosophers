NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g
SRC = philo.c utils.c jobs.c philo_live.c utils2.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

fclean:
	rm -f $(NAME)
	rm -f $(OBJ)

re: fclean all
