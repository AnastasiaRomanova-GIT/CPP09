NAME        = RPN

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
DBGFLAGS    = -g -Wall -Wextra -Werror -std=c++98

SRCSDIR     = src
OBJDIR      = build

SRCFILES	= \
	main.cpp \
	RPN.cpp


SRCS        = $(addprefix $(SRCSDIR)/,$(SRCFILES))
OBJS        = $(patsubst $(SRCSDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# ---------- default ----------

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# ---------- object compilation ----------

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- extra targets ----------

exec_only: $(OBJS)
	rm -f $(NAME)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

debug:
	rm -f $(NAME)
	$(CXX) $(DBGFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re exec_only debug
