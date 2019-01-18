# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/10 13:21:18 by jplevy            #+#    #+#              #
#    Updated: 2019/01/18 13:18:14 by opus1io          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm
CC = gcc
CFLAG = -Wall -Werror -Wextra
SRC_PATH = ./srcs/
OBJ_PATH = ./dst/
SRC_NAME = ft_nm.c \
			errors.c \
			flags.c \
			symbols.c \
			sections.c \
			main.c \
			merge_sort.c
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
INCLUDE_FILE = ./includes/ft_nm.h
LIBFT_PATH = ./../libft/
LIBFT_NAME = libft.a
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Creating libft"
	@$(MAKE) -C $(LIBFT_PATH) > /dev/null
	@$(CC) -o $(NAME) $(OBJ) -L. $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "  Well done"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c  $(INCLUDE_FILE)
	@echo $@
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAG) -o $@ -c $<  -I./includes
	@echo "  Well done"

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(RM) $(OBJ_PATH)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)

re: fclean all
