# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: a-soeiro <marvavieira-@student.42porto.com +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/22 01:45:12 by a-soeiro          #+#    #+#              #
#    Updated: 2025/09/14 15:15:39 by a-soeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= pipex
B_NAME= pipex_bonus
CC= cc -g
CFLAGS= -Wall -Wextra -Werror
INCLUDES= ./includes
B_INCLUDES= ./bonus/includes
LIBFT_DIR= ./libs/libft
LIBFT= $(LIBFT_DIR)/libft.a
SRCS_DIR= ./srcs
B_SRCS_DIR= ./bonus/srcs
SRCS= $(SRCS_DIR)/pipex.c \
$(SRCS_DIR)/pipex_utils.c
B_SRCS= $(B_SRCS_DIR)/pipex_bonus.c \
$(B_SRCS_DIR)/pipex_utils_bonus.c \
$(B_SRCS_DIR)/pipe_init_bonus.c \
$(B_SRCS_DIR)/pipe_list_bonus.c \
$(B_SRCS_DIR)/pipe_clering_bonus.c

all: $(LIBFT) $(NAME)

$(NAME): $(LIBFT)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

bonus: .bonus

.bonus: $(LIBFT)
	$(CC) $(CFLAGS) $(B_SRCS) $(LIBFT) -o $(B_NAME)
	touch .bonus

%.o : %.c $(CC) $(CFLAGS) -I $(INCLUDES) -c $^ -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)

bonus_fclean: clean
	rm -rf $(B_NAME)
	rm -rf .bonus

bonus_re: bonus_fclean bonus

re: fclean all

.PHONY: all clean fclean re
