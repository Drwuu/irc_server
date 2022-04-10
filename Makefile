NAME			=  irc_42

FLAGS			= -Wall -Wextra -Werror -std=c++98
FLAGS			+= -g3 -fsanitize=address

OBJS_DIR		= objects
INCLUDES		= headers

SRCS			= \
$(addprefix parser/, parser_tester.cpp parser.cpp) \
$(addprefix commands/, command.cpp admin.cpp) \

HEADERS			= \
$(addprefix commands/, command.hpp admin.hpp) \
$(addprefix parser/, parser.hpp) \
irc.hpp \

SOURCES			= sources
FOLDERS			= commands parser
OBJS			= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

all:				$(NAME)

server_test:		./server_test/server_test.cpp
						clang++ $< -o $@_
						./$@_

clean_server_test:
					rm -f server_test_
					rm -f ./server_test/*.o

$(NAME):			$(OBJS)
						clang++ $(FLAGS) $^ -o $@
						printf "\033[2K\r$(BLUE)$(NAME)$(RESET)$(BLUE): $(GREEN)Compiled [√]$(RESET)$(NO_COLOR)\n"
						./$(NAME)

$(OBJS_DIR)/%.o:	$(SOURCES)/%.cpp $(addprefix $(INCLUDES)/, $(HEADERS))
						mkdir -p $(addprefix $(OBJS_DIR)/, $(FOLDERS))
						clang++ $(FLAGS) -I $(INCLUDES) -c $< -o $@
						printf "\033[2K\r$(BLUE)$(NAME)$(RESET)$(BLUE): $(PURPLE)$<$(RESET)$(NO_COLOR)"

clean:
						rm -rf $(OBJS)
						rm -rf $(addprefix $(OBJS_DIR)/, $(FOLDERS))
						rm -rf $(OBJS_DIR)

fclean:				clean
						rm -f $(NAME)

re:					fclean all

.PHONY:				all clean fclean re $(NAME)
.SILENT:

BLACK			=	\033[0;30m
RED				=	\033[0;31m
GREEN			=	\033[0;32m
ORANGE			=	\033[0;33m
BLUE			=	\033[0;34m
PURPLE			=	\033[0;35m
CYAN			=	\033[0;36m
LIGHT_GRAY		=	\033[0;37m
DARK_GRAY		=	\033[1;30m
LIGHT_RED		=	\033[1;31m
LIGHT_GREEN		=	\033[1;32m
YELLOW			=	\033[1;33m
LIGHT_BLUE		=	\033[1;34m
LIGHT_PURPLE	=	\033[1;35m
LIGHT_CYAN		=	\033[1;36m
WHITE			=	\033[1;37m
NO_COLOR		=	\033[0m
