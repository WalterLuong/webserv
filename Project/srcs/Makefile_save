NAME    =	Parsing

# --   Directory   -- #
OBJ_DIR     =   objs
INC_DIR        =    ../include

# --  Search All files in SRCS  -- #
SRC_DIR     =    $(shell find . -type d)
vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

SRCS =   main.cpp	\
		 parsing.cpp \
		 parsing_conffile.cpp \
		 utils_parsing.cpp	\
		 parsing_server.cpp \
		 parsing_request.cpp	\


# --  Redirection in OBJS  -- #
OBJS    = $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

# --   Compilation flags  -- #
CC        = clang++
CPPFLAG    = -Wall -Wextra -Werror -std=c++98

.PHONY: all

all:
	@make $(NAME) --no-print-directory

$(NAME): $(OBJS)
		$(CC) $(CPPFLAG) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o : %.cpp | $(OBJ_DIR)
	${CC} ${CPPFLAG} -o $@ -c $< -I $(INC_DIR)

$(OBJ_DIR) :
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)

.PHONY: fclean

fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all
