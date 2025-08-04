# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: a-soeiro <avieira-@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/03 21:37:38 by a-soeiro          #+#    #+#              #
#    Updated: 2025/08/03 21:48:26 by a-soeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= pipex
CC= cc
CFLAGS= -Wall -Wextra -Werror
INCLUDES= ./includes
LIBFT_DIR= ./libs/libft
SRCS_DIR= ./srcs

SRCS= $(SRCS_DIR)/main.c \
$(SRCS)/

$(NAME):

%.o : %.c $(CC) $(CFLAGS) -I $(INCLUDES) -c $^ -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJS)
	make cleana -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
