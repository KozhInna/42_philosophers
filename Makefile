#Program name
NAME := philo

CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread -I./include

SRC_DIR :=src/
OBJS_DIR := objs

SOURCE := $(SRC_DIR)/main.c
HEADERS = include/philo.h

OBJS = $(SOURCE:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR) 

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re