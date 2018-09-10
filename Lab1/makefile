CXX = g++-8
LINK_FLAGS = -lglfw
SRCS = $(wildcard *.cpp) $(wildcard **/*.cpp)
HEDS = $(wildcard *.h) $(wildcard **/*.h)
OBJS = $(SRCS:.cpp=.o)
DEPS = dependencies.dep

all: lab1
	echo All done!

lab1: glad.o $(OBJS)
	$(CXX) $^ $(LINK_FLAGS) -o $@

$(DEPS): $(SRCS) $(HEDS)
	@$(CXX) -MM $(SRCS) > $(DEPS)

glad.o: glad.c
	gcc glad.c -c

-include $(DEPS)

clean:
	find . -type f -name '*.o' -delete
	rm -f lab1 dependencies.dep