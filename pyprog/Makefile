NAME		:= isomorphisme
BUILD_DIR	:= .build
SRC_DIR		:= src
SRC_EXT		:= .c
SRC			:= $(wildcard $(SRC_DIR)/*$(SRC_EXT))
DATA_DIR	:= data
CC			:= gcc
CFLAGS		:= -Wall -Ofast -Ilib
LDFLAGS		:= -lm -Llib -l:nauty.a
OBJ_EXT		:= .o
OBJ			:= $(SRC:$(SRC_DIR)/%$(SRC_EXT)=$(BUILD_DIR)/%.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run: $(NAME)
	./$(NAME)

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%$(SRC_EXT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(NAME)
	rm -rf $(DATA_DIR)
	mkdir $(DATA_DIR)
	clear

-include $(OBJ:%$(OBJ_EXT)=%.d)
