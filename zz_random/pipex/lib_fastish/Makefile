# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/07 20:59:39 by ltaalas           #+#    #+#              #
#    Updated: 2025/01/16 17:58:33 by ltaalas          ###   ########.fr        #
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

NAME = $(BIN_DIR)/fastish.a
 
SOURCES = ft_calloc_fastish.c ft_memmove_fastish.c ft_memset_fastish.c ft_realloc_fastish.c

SRC = $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

HEADERS = $(addprefix $(INC_DIR)/, fastish.h)

CC_FLAGS = -Wall -Wextra -Werror -O3

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
	@echo "Built to lib_fastish/$@"

$(BIN_DIR):
	@echo -n $(BOLD_ORANGE) && echo "Making $@ directory" && echo -n $(RESET_COLOR)
	@mkdir -p $@
	@echo -n $(BOLD_LIGHT_BLUE) && echo "Made $@ directory" && echo -n $(RESET_COLOR)

clean:
	@echo -n $(BOLD_ORANGE) && echo "Cleaning lib_fastish/$(OBJ_DIR)/" && echo -n $(RESET_COLOR)
	@rm -f $(OBJECTS)

fclean: clean
	@echo -n $(UNDERSCORE_ORANGE)
	@echo "Deleting lib_fastish/$(NAME)"
	@rm -f $(NAME)
	@echo -n $(RESET_COLOR)


re: fclean all

.PHONY: all clean fclean re bonus


