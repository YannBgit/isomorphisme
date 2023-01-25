BUILD_DIR	:= .build
CC			:= gcc
CFLAGS		:= -Wall -Ofast -Ilib -Isrc
SRC_DIR		:= src
SRC_EXT		:= .c
SRC_FILES	:= *.sdf
LDFLAGS		:= -lm -Llib -l:nauty.a -g
OBJ_EXT		:= .o
DATA_DIR	:= data

SRC			:= $(wildcard $(SRC_DIR)/*$(SRC_EXT))
OBJ			:= $(SRC:$(SRC_DIR)/%$(SRC_EXT)=$(BUILD_DIR)/%.o)

NAME_CREATE		:= create_index
OBJ_CREATE			:= $(BUILD_DIR)/$(NAME_CREATE).o

NAME_SEARCH		:= search_index
OBJ_SEARCH			:= $(BUILD_DIR)/$(NAME_SEARCH).o

INDEX_FILE	:= index.txt

all: $(NAME_CREATE) $(NAME_SEARCH)

setup:
	mkdir -p $(DATA_DIR)

run: $(INDEX_FILE) $(NAME_SEARCH)
	./$(NAME_SEARCH)

$(INDEX_FILE): $(NAME_CREATE)
	./$(NAME_CREATE)

$(NAME_CREATE): $(OBJ) $(OBJ_CREATE)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(NAME_SEARCH): $(OBJ) $(OBJ_SEARCH)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%$(SRC_EXT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/mains/%$(SRC_EXT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(NAME_CREATE) $(NAME_SEARCH) $(INDEX_FILE)
	rm -rf $(BUILD_DIR)
	rm -rf $(DATA_DIR)
	rm -f $(SRC_FILES)
	mkdir $(DATA_DIR)
	clear

-include $(OBJ:%$(OBJ_EXT)=%.d)