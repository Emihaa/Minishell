# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/01 16:02:01 by ltaalas           #+#    #+#              #
#    Updated: 2025/03/24 17:39:21 by ehaanpaa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET_COLOR = echo -n "\e[0m"

GREEN = echo -n "\e[0;32m"
GREY = echo -n "\e[0;37m"

BOLD_LIGHT_BLUE = echo -n "\e[1;36m"
BOLD_RED = echo -n "\e[1;31m"
BOLD_PURPLE = echo -n "\e[1;35m"
BOLD_ORANGE = echo -n "\e[1;33m"
BOLD_GREEN = echo -n "\e[1;32m"

UNDERSCORE_ORANGE = echo -n "\e[4;33m"

SRC_DIR = sources
BIN_DIR = build
OBJ_DIR = objects
INC_DIR = includes

NAME = lt_alloc.a
 
SOURCES = 	lt_alloc_new_delete.c	lt_alloc_arena.c	lt_alloc_arena_temp.c

HEADERS = $(addprefix $(INC_DIR)/, lt_alloc.h)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
BONUS_OBJECTS = $(addprefix $(OBJ_DIR)/, $(BONUS_SOURCES:.c=.o))

CC_FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	@$(GREEN)
	cc $(CC_FLAGS) -c $< -o $@
	@$(RESET_COLOR)

$(OBJ_DIR):
	@$(BOLD_ORANGE)
	@echo "Making $@ directory"
	@$(RESET_COLOR)
	@mkdir -p $@
	@$(BOLD_LIGHT_BLUE)
	@echo "Made $@ directory"
	@$(RESET_COLOR)

$(NAME): $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJECTS) | $(BIN_DIR)
	@$(BOLD_GREEN)
	@echo "Archiving $@"
	@ar -rcs $(BIN_DIR)/$(NAME) $(OBJECTS)
	@$(RESET_COLOR)
	@echo "Built to $@"

$(BIN_DIR):
	@$(BOLD_ORANGE)
	@echo "Making $@ directory"
	@$(RESET_COLOR)
	@mkdir -p $@
	@$(BOLD_LIGHT_BLUE)
	@echo "Made $@ directory"
	@$(RESET_COLOR)

clean:
	@$(BOLD_ORANGE)
	@echo "Cleaning $(NAME)/$(OBJ_DIR)/"
	@rm -f $(OBJECTS)
	@$(RESET_COLOR)


fclean: clean
	@$(UNDERSCORE_ORANGE)
	@echo "Deleting $(BIN_DIR)/$(NAME)"
	@rm -f $(BIN_DIR)/$(NAME)
	@$(RESET_COLOR)


re: fclean all

.PHONY: libft all clean fclean re bonus
