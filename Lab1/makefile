CXX = g++-8
LINK_FLAGS = -lglfw
# SRCS = $(wildcard *.cpp) $(wildcard **/*.cpp)
SRCS = $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
MAIN_SRCS = $(wildcard *.cpp)
ALL_SRCS = $(SRCS) $(MAIN_SRCS)
HEDS = $(wildcard *.h) $(wildcard **/*.h)
OBJS = $(SRCS:.cpp=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
DEPS = dependencies.dep

.PHONY: all
all: testWindow lab1

testWindow: mainWindow.o glad.o $(OBJS)
	$(CXX) $^ $(LINK_FLAGS) -o $@

lab1: lab1.o glad.o $(OBJS)
	$(CXX) $^ $(LINK_FLAGS) -o $@

$(DEPS): $(ALL_SRCS) $(HEDS)
	@$(CXX) -MM $(ALL_SRCS) > $(DEPS)

glad.o: glad.c
	gcc glad.c -c

-include $(DEPS)

.PHONY: clean
clean:
	find . -type f -name '*.o' -delete
	rm -f lab1 testWindow dependencies.dep