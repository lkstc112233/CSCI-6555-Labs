CXXFLAGS = -std=c++17 
LINK_FLAGS = -L/usr/local/lib -lglfw
# SRCS = $(wildcard *.cpp) $(wildcard **/*.cpp)
SRCS = $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
MAIN_SRCS = $(wildcard *.cpp)
ALL_SRCS = $(SRCS) $(MAIN_SRCS)
HEDS = $(wildcard *.h) $(wildcard **/*.h)
OBJS = $(SRCS:.cpp=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
DEPS = dependencies.dep

.PHONY: all
all: lab4

lab4: lab4.o glad.o $(OBJS)
	@echo Making lab4.
	@$(CXX) $^ $(LINK_FLAGS) -o $@

$(DEPS): $(ALL_SRCS) $(HEDS)
	@$(CXX) -MM $(ALL_SRCS) > $(DEPS)

glad.o: glad.c
	gcc glad.c -c

-include $(DEPS)

.PHONY: clean
clean:
	find . -type f -name '*.o' -delete
	rm -f lab4 dependencies.dep
