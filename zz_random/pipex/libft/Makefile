# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/01 16:02:01 by ltaalas           #+#    #+#              #
#    Updated: 2025/02/23 16:58:18 by ltaalas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET_COLOR = "\e[0m"

GREEN = "\e[0;32m"
GREY = "\e[0;37m"

BOLD_LIGHT_BLUE = "\e[1;36m"
BOLD_RED = "\e[1;31m"
BOLD_PURPLE = "\e[1;35m"
BOLD_ORANGE = "\e[1;33m"
BOLD_GREEN = "\e[1;32m"

UNDERSCORE_ORANGE = "\e[4;33m"



SRC_DIR = sources
BIN_DIR = build
OBJ_DIR = objects
INC_DIR = includes

NAME = $(BIN_DIR)/libft.a
BONUS_NAME = $(BIN_DIR)/.libft_bonus
 
SOURCES = 	ft_is_white_space.c ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
			ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
			ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_putstr_fd.c ft_split.c ft_strchr.c ft_strrchr.c ft_strdup.c ft_striteri.c \
			ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
			ft_strnstr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c get_next_line.c

BONUS_SOURCES = 	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
					ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
					ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

INCLUDES = get_next_line.h libft.h

HEADERS = $(addprefix $(INC_DIR)/, libft.h)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
BONUS_OBJECTS = $(addprefix $(OBJ_DIR)/, $(BONUS_SOURCES:.c=.o))

CC_FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	@echo -n $(GREEN)
	cc $(CC_FLAGS) -c $< -o $@
	@echo -n $(RESET_COLOR)

$(OBJ_DIR):
	@echo -n $(BOLD_ORANGE) && echo "Making $@ directory" && echo -n $(RESET_COLOR)
	@mkdir -p $@
	@echo -n $(BOLD_LIGHT_BLUE) && echo "Made $@ directory" && echo -n $(RESET_COLOR)

$(NAME): $(OBJECTS) | $(BIN_DIR)
	@echo -n $(BOLD_GREEN)
	@echo "Archiving $@"
	@ar -rcs $(NAME) $(OBJECTS)
	@echo -n $(RESET_COLOR)
	@echo "Built to libft/$@"

$(BIN_DIR):
	@echo -n $(BOLD_ORANGE) && echo "Making $@ directory" && echo -n $(RESET_COLOR)
	@mkdir -p $@
	@echo -n $(BOLD_LIGHT_BLUE) && echo "Made $@ directory" && echo -n $(RESET_COLOR)

clean:
	@echo -n $(BOLD_ORANGE)
	@echo "Cleaning libft/$(OBJ_DIR)/"
	@rm -f $(OBJECTS) $(BONUS_OBJECTS) $(BONUS_NAME)
	@echo -n $(RESET_COLOR)

fclean: clean
	@echo -n $(UNDERSCORE_ORANGE) && echo "Deleting libft/$(NAME)" && echo -n $(RESET_COLOR)
	@rm -f $(NAME) $(BONUS_NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJECTS) $(BONUS_OBJECTS)
	ar -rcs $(NAME) $(OBJECTS) $(BONUS_OBJECTS)
	touch $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
