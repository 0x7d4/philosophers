# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmomeni <mmomeni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 19:15:52 by mmomeni           #+#    #+#              #
#    Updated: 2023/09/16 13:54:41 by mmomeni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=		philo
CFLAGS=		-Wall -Wextra -Werror -pthread
FUNCS=		threads.c monitor.c parse.c utils.c
SRCS=		main.c $(FUNCS)

BONUS_DIR=	philo_bonus
BONUS_NAME=	philo
BONUS_SRCS=	$(BONUS_DIR)/main_bonus.c

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

bonus: $(BONUS_NAME)
	$(CC) $(CFLAGS) $(FUNCS) $(BONUS_SRCS) -o $(BONUS_NAME)
	
clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(BONUS_NAME)

re: fclean all

race: CFLAGS += -fsanitize=thread -g
race: $(NAME)

leak: CFLAGS += -fsanitize=address -g
leak: $(NAME)

debug: CFLAGS += -g
debug: $(NAME)

.PHONY: all clean fclean re bonus
