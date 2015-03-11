SRC_SRV = src_server/server.c

SRC_CLT = src_client/client.c

OBJ_SRV = $(SRC_SRV:.c=.o)

OBJ_CLT = $(SRC_CLT:.c=.o)

NAME_SRV = server

NAME_CLT = client

RM = rm -f

CFLAGS += -Wall -Werror -Wextra

all:	$(NAME_SRV) $(NAME_CLT)

$(NAME_SRV):$(OBJ_SRV)
	cc -o $(NAME_SRV) $(OBJ_SRV)

$(NAME_CLT):$(OBJ_CLT)
	cc -o $(NAME_CLT) $(OBJ_CLT)

clean:
	$(RM) $(OBJ_SRV) $(OBJ_CLT)

fclean: clean
	$(RM) $(NAME_SRV) $(NAME_CLT)

re: fclean all

.PHONY: all clean fclean re
