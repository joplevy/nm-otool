# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/10 13:21:18 by jplevy            #+#    #+#              #
#    Updated: 2019/04/17 17:20:54 by opus1io          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM_NAME = ft_nm
OT_NAME = ft_otool

CC = clang
CFLAG = -Wall -Werror -Wextra

NM_PATH = ./nm/
OT_PATH = ./otool/
SRC_PATH = ./srcs/
OBJ_PATH = ./dst/

NM_SRC_NAME = ft_nm.c \
			errors.c \
			flags.c \
			symbols.c \
			sections.c \
			main.c \
			merge_sort.c
NM_OBJ_NAME = $(NM_SRC_NAME:.c=.o)
NM_OBJ_C = $(addprefix $(OBJ_PATH), $(NM_OBJ_NAME))
NM_OBJ = $(addprefix $(NM_PATH), $(NM_OBJ_C))

OT_SRC_NAME = main.c
OT_OBJ_NAME = $(OT_SRC_NAME:.c=.o)
OT_OBJ_C = $(addprefix $(OBJ_PATH), $(OT_OBJ_NAME))
OT_OBJ = $(addprefix $(OT_PATH), $(OT_OBJ_C))

NM_INCLUDE_FILE = ./includes/ft_nm.h
OT_INCLUDE_FILE = ./includes/ft_nm.h

LIBFT_PATH = ./libft/
LIBFT_NAME = libft.a
RM = rm -rf

all: $(NM_NAME) $(OT_NAME)

$(NM_NAME): $(NM_OBJ)
	@echo "Creating libft"
	@$(MAKE) -C $(LIBFT_PATH) > /dev/null
	@$(CC) -o $(NM_NAME) $(NM_OBJ) -L. $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "  Well done"

$(OT_NAME): $(OT_OBJ)
	@echo "Creating libft"
	@$(MAKE) -C $(LIBFT_PATH) > /dev/null
	@$(CC) -o $(OT_NAME) $(OT_OBJ) -L. $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "  Well done"

$(OT_PATH)$(OBJ_PATH)%.o: $(OT_PATH)$(SRC_PATH)%.c  $(OT_INCLUDE_FILE)
	@echo $@
	@mkdir -p $(OT_PATH)$(OBJ_PATH)
	@$(CC) $(CFLAG) -o $@ -c $<  -I./includes
	@echo "  Well done"

$(NM_PATH)$(OBJ_PATH)%.o: $(NM_PATH)$(SRC_PATH)%.c  $(NM_INCLUDE_FILE)
	@echo $@
	@mkdir -p $(NM_PATH)$(OBJ_PATH)
	@$(CC) $(CFLAG) -o $@ -c $<  -I./includes
	@echo "  Well done"

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(RM) $(NM_PATH)$(OBJ_PATH)
	@$(RM) $(OT_PATH)$(OBJ_PATH)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NM_NAME)
	@$(RM) $(OT_NAME)

re: fclean all
