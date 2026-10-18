NAME        = PmergeMe

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
DBGFLAGS    = -g -Wall -Wextra -Werror -std=c++98

SRCSDIR     = src
OBJDIR      = build

SRCFILES	= \
	main.cpp \
	PmergeMe.cpp \
	list/listSortMain.cpp \
	list/utilsList.cpp \
	vector/vectorSortMain.cpp \
	vector/utilsVector.cpp 


SRCS        = $(addprefix $(SRCSDIR)/,$(SRCFILES))
OBJS        = $(patsubst $(SRCSDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS        = $(OBJS:.o=.d)

# ---------- default ----------

all: $(OBJS)
	rm -f $(NAME)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# ---------- object compilation ----------

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# ---------- extra targets ----------

exec_only: $(OBJS)
	rm -f $(NAME)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

debug:
	rm -f $(NAME)
	$(CXX) $(DBGFLAGS) $(OBJS) -o $(NAME)

count_operations: CXXFLAGS = $(DBGFLAGS) -DCOUNTCOMPARISONS
count_operations: clean $(OBJS)
	rm -f $(NAME)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

counterON: count_operations

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re exec_only debug counterON

-include $(DEPS)
