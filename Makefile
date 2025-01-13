NAME = ft_ping

CC = gcc

FLAGS = -Wall -Wextra -Werror

SRC_FOLDER = src

OBJ_FOLDER = src

SRC = src/utils.c src/ft_ping.c src/main.c src/parser.c

HEADER = src/ft_ping.h

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME):$(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) -lm 

$(OBJ_FOLDER)/%.o:$(OBJ_FOLDER)/%.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re: fclean  all