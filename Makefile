NAME			=  irc_42

MAIN			:= main_server.cpp

ARGS			:= 4242 lol

FLAGS			= -Wall -Wextra -Werror -std=c++98
FLAGS			+= -g3 -fsanitize=address

OBJS_DIR		= objects
INCLUDES		= headers


SRCS			= \
				  $(addprefix commands/, command.cpp invite.cpp kick.cpp mode.cpp User_cmd.cpp Nick.cpp Message.cpp) \
				  $(addprefix error/, error.cpp) \
				  $(addprefix parser/, parser.cpp) \
				  $(addprefix structure/, Server.cpp User.cpp Channel.cpp) \
				  $(addprefix proxy/, Address.cpp Socket_event.cpp Server_queue.cpp Proxy_queue.cpp Proxy.cpp)

HEADERS			= \
				  irc.hpp \
				  $(addprefix structure/, Server.hpp User.hpp Channel.hpp) \
				  $(addprefix commands/, command.hpp invite.hpp kick.hpp mode.hpp User_cmd.hpp Nick.hpp Message.hpp) \
				  $(addprefix error/, error.hpp) \
				  $(addprefix parser/, parser.hpp) \
				  $(addprefix proxy/, Address.hpp Socket_event.hpp Server_queue.hpp Proxy_queue.hpp Proxy.hpp)

PATH_HEADERS	= $(addprefix $(INCLUDES)/, $(HEADERS))

SOURCES			= sources
FOLDERS			= commands error parser structure proxy
OBJS			= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

all:				$(NAME)

server_test:		./server_test/server_test.cpp
						c++ $< -o $@_
						./$@_
clean_server_test:
					rm -f server_test_
					rm -f ./server_test/*.o

exec:				$(NAME)
						./$(NAME) $(ARGS)

$(NAME):			$(OBJS)
						clang++ $(FLAGS) $(MAIN) $^ -o $@
						printf "\033[2K\r$(BLUE)$(NAME)$(RESET)$(BLUE): $(GREEN)Compiled [√]$(RESET)$(NO_COLOR)\n"

$(OBJS_DIR)/%.o:	$(SOURCES)/%.cpp $(PATH_HEADERS)
						mkdir -p $(addprefix $(OBJS_DIR)/, $(FOLDERS))
						c++ $(FLAGS) -I $(INCLUDES) -c $< -o $@
						printf "\033[2K\r$(BLUE)$(NAME)$(RESET)$(BLUE): $(PURPLE)$<$(RESET)$(NO_COLOR)"

clean:
						rm -rf $(OBJS)
						rm -rf $(addprefix $(OBJS_DIR)/, $(FOLDERS))
						rm -rf $(OBJS_DIR)

fclean:				clean
						rm -f $(NAME)

re:					fclean all

.PHONY:				all clean fclean re $(NAME) server_test clean_server_test
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
