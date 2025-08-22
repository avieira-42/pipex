# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: a-soeiro <marvavieira-@student.42porto.com +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/22 01:45:12 by a-soeiro          #+#    #+#              #
#    Updated: 2025/08/22 03:48:18 by a-soeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= pipex
CC= cc
CFLAGS= -Wall -Wextra -Werror
INCLUDES= ./includes
LIBFT_DIR= ./libs/libft
LIBFT= $(LIBFT_DIR)/libft.a
SRCS_DIR= ./srcs
SRCS= $(SRCS_DIR)/pipex.c \
$(SRCS_DIR)/pipex_utils.c

$(NAME): $(LIBFT)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

%.o : %.c $(CC) $(CFLAGS) -I $(INCLUDES) -c $^ -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
