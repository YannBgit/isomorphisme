NAME		:= isomorphisme.out
BUILD_DIR	:= .build
SRC_DIR		:= src
SRC_EXT		:= .c
SRC			:= $(wildcard $(SRC_DIR)/*$(SRC_EXT))
CC			:= gcc
CFLAGS		:= -Wall -Ofast
LDFLAGS		:= -lm
RUN_ARGS	:=
OBJ_EXT		:= .o
OBJ			:= $(SRC:$(SRC_DIR)/%$(SRC_EXT)=$(BUILD_DIR)/%.o)

compile: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR) $(NAME)
	clear

re: clean all

run: compile
	./$< $(RUN_ARGS)

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%$(SRC_EXT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(OBJ:%$(OBJ_EXT)=%.d)

.PHONY: all clean re run