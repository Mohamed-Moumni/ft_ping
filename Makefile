NAME = ft_ping

CPP = gcc

FLAGS = -Wall -Wextra -Werror

SRC = src/parser.c src/utils.c src/ft_ping.c main.c

HEADER = ft_ping.h

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME):$(OBJ)
	$(CPP) $(FLAGS) -o $(NAME) $(OBJ) -lm 

%.o:%.c $(HEADER)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re: fclean  all