# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/01 17:30:48 by ltaalas           #+#    #+#              #
#    Updated: 2025/03/17 23:28:31 by ehaanpaa         ###   ########.fr        #
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

LIB_DIR = libs

LIBFT_DIR = libft
LIBFT = $(LIB_DIR)/$(LIBFT_DIR)/build/libft.a

FASTISH_DIR = lib_fastish
FASTISH = $(LIB_DIR)/$(FASTISH_DIR)/build/fastish.a

LT_ALLOC_DIR = lt_alloc
LT_ALLOC = $(LIB_DIR)/$(LT_ALLOC_DIR)/build/lt_alloc.a

NAME = minishell

SOURCES = minishell.c redirect.c lexer.c heredoc.c tree.c

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

HEADERS = $(addprefix $(INC_DIR)/, minishell.h)

LINKS = $(addprefix -l, readline) # make this make any sense

CC_FLAGS = -Wall -Wextra -Werror 

all: $(NAME)

libft:
	@echo "Cheking libft.a"
	@$(BOLD_PURPLE)
	@make --no-print-directory -C $(LIB_DIR)/$(LIBFT_DIR)/ CC_FLAGS="$(CC_FLAGS)"
	@$(RESET_COLOR)

fastish:
	@echo "Cheking fastish.a"
	@$(BOLD_PURPLE)
	@make --no-print-directory -C $(LIB_DIR)/$(FASTISH_DIR)/ CC_FLAGS="$(CC_FLAGS)"
	@$(RESET_COLOR)

lt_alloc:
	@echo "Cheking $@.a"
	@$(BOLD_PURPLE)
	@make --no-print-directory -C $(LIB_DIR)/$(LT_ALLOC_DIR)/ CC_FLAGS="$(CC_FLAGS)"
	@$(RESET_COLOR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	@$(GREEN) 
	cc $(CC_FLAGS) -c $< -o $@
	@$(RESET_COLOR)

$(OBJ_DIR):
	@$(BOLD_ORANGE)
	@echo -n "Making $@ directory"
	$(RESET_COLOR)
	@mkdir -p $@
	$(BOLD_LIGHT_BLUE)
	@echo -n "Made $@ directory"
	@$(RESET_COLOR)

$(NAME): lt_alloc libft $(OBJECTS) | $(BIN_DIR)
	@make --no-print-directory $(BIN_DIR)/$(NAME)

#	restucture to just have one LIBS or DEPENDENCIES variable

$(BIN_DIR)/$(NAME): $(OBJECTS) $(LT_ALLOC) $(LIBFT) | $(BIN_DIR)
	@$(BOLD_GREEN)
	@echo "Linking $(NAME)"
	@cc $(CC_FLAGS) $(OBJECTS) $(LT_ALLOC) $(LIBFT) $(LINKS) \
	-o $(BIN_DIR)/$(NAME)
	@$(RESET_COLOR)
	@echo "Built to $(BIN_DIR)/$(NAME)"

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
	@$(RESET_COLOR)
	rm -f $(OBJECTS)
	@make -C $(LIB_DIR)/$(LT_ALLOC_DIR) clean --no-print-directory
	 @make -C $(LIB_DIR)/$(LIBFT_DIR) clean --no-print-directory
# @make -C $(FASTISH_DIR) clean --no-print-directory
	@$(RESET_COLOR)

fclean:
	@$(BOLD_ORANGE) 
	@echo "Cleaning $(OBJ_DIR)/$(NAME)"
	@$(RESET_COLOR)
	rm -f $(OBJECTS)
	@$(UNDERSCORE_ORANGE)
	@echo "Deleting $(BIN_DIR)/$(NAME)"
	@$(RESET_COLOR)
	rm -f $(BIN_DIR)/$(NAME)
	@make -C $(LIB_DIR)/$(LT_ALLOC_DIR) fclean --no-print-directory
	@make -C $(LIB_DIR)/$(LIBFT_DIR) fclean --no-print-directory 
# @make -C $(FASTISH_DIR) fclean --no-print-directory
	@$(RESET_COLOR)

re: fclean all

no_built_in_optimize: CC_FLAGS += -fno-builtin
no_built_in_optimize: optimize

no_built_in_profile: CC_FLAGS += -fno-builtin -pg -O3
no_built_in_profile: all

debug: CC_FLAGS += -g3 -Wunreachable-code -Wunused-function
debug: all

profile: CC_FLAGS += -O3 -pg 
profile: all

optimize: CC_FLAGS += -O3 -mavx2 -msse4.2 -maes
optimize: all

.PHONY: all clean fclean re 
.PHONY: debug profile optimize no_built_in_optimize no_built_in_profile
.PHONY: libft fastish
