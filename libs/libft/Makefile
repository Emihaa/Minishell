# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/01 16:02:01 by ltaalas           #+#    #+#              #
#    Updated: 2025/02/27 19:41:12 by ltaalas          ###   ########.fr        #
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

NAME = $(BIN_DIR)/libft.a
BONUS_NAME = $(BIN_DIR)/.libft_bonus
 
SOURCES = 			ft_is_white_space.c	ft_atoi.c		ft_calloc.c		get_next_line.c	\
					ft_isalnum.c		ft_isalpha.c	ft_isascii.c 	ft_isdigit.c	\
					ft_isprint.c		ft_itoa.c		ft_memchr.c		ft_memcmp.c		\
					ft_memcpy.c			ft_memmove.c	ft_memset.c		ft_bzero.c		\
					ft_putchar_fd.c 	ft_putendl_fd.c ft_putnbr_fd.c	ft_putstr_fd.c	\
					ft_split.c			ft_strdup.c		ft_strchr.c		ft_strrchr.c 	\
					ft_strjoin.c		ft_striteri.c	ft_strlcat.c	ft_strlcpy.c	\
					ft_strlen.c			ft_strmapi.c	ft_strncmp.c	ft_strnstr.c 	\
					ft_strtrim.c		ft_substr.c		ft_tolower.c	ft_toupper.c	\

BONUS_SOURCES = 	ft_lstnew_bonus.c	ft_lstadd_front_bonus.c	ft_lstsize_bonus.c \
					ft_lstlast_bonus.c	ft_lstadd_back_bonus.c	ft_lstdelone_bonus.c \
					ft_lstclear_bonus.c	ft_lstiter_bonus.c		ft_lstmap_bonus.c

INCLUDES =			get_next_line.h	libft.h

HEADERS = $(addprefix $(INC_DIR)/, libft.h)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
BONUS_OBJECTS = $(addprefix $(OBJ_DIR)/, $(BONUS_SOURCES:.c=.o))

CC_FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(GREEN)
	cc $(CC_FLAGS) -c $< -o $@
	$(RESET_COLOR)

$(OBJ_DIR):
	$(BOLD_ORANGE) && echo "Making $@ directory"
	$(RESET_COLOR)
	@mkdir -p $@
	$(BOLD_LIGHT_BLUE) && echo "Made $@ directory"
	$(RESET_COLOR)

$(NAME): $(OBJECTS) | $(BIN_DIR)
	$(BOLD_GREEN)
	@echo "Archiving $@"
	@ar -rcs $(NAME) $(OBJECTS)
	$(RESET_COLOR)
	@echo "Built to libft/$@"

$(BIN_DIR):
	$(BOLD_ORANGE)
	@echo -n "Making $@ directory"
	$(RESET_COLOR)
	@mkdir -p $@
	$(BOLD_LIGHT_BLUE)
	@echo -n "Made $@ directory"
	$(RESET_COLOR)

clean:
	$(BOLD_ORANGE)
	@echo "Cleaning libft/$(OBJ_DIR)/"
	@rm -f $(OBJECTS) $(BONUS_OBJECTS) $(BONUS_NAME)
	$(RESET_COLOR)

fclean: clean
	$(UNDERSCORE_ORANGE)
	@echo "Deleting libft/$(NAME)"
	@rm -f $(NAME) $(BONUS_NAME)
	$(RESET_COLOR)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJECTS) $(BONUS_OBJECTS)
	ar -rcs $(NAME) $(OBJECTS) $(BONUS_OBJECTS)
	touch $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
