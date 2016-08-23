
RM	= rm -f

NAME	= cll1p.so

CFLAGS	= -Wall -Wall -Werror

SRCS	= cll1p.c \
	  cll1p_token.c

OBJS	= $(SRCS:.c=.o)


all:	$(NAME)

$(NAME): $(OBJS)
	gcc -c -fPIC $(SRCS)
	gcc -shared -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
