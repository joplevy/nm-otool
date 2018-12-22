# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/10 13:21:18 by jplevy            #+#    #+#              #
#    Updated: 2018/12/22 12:54:44 by opus1io          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM_NAME = ft_nm
NM_PATH = ./nm/
RM = rm -f

all: $(NM_NAME)

$(NM_NAME):
	@$(MAKE) -C $(NM_PATH)
	@echo "Copying " $@
	@cp $(NM_PATH)$(NM_NAME) .
	@echo "  Well done"

clean:
	@$(MAKE) -C $(NM_PATH) clean

fclean: clean
	@$(MAKE) -C $(NM_PATH) fclean
	@$(RM) $(NM_NAME)

re: fclean all
