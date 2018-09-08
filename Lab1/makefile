LINK_FLAGS = -lglfw

lab1: glad.o mainWindow.o
	g++ glad.o mainWindow.o $(LINK_FLAGS) -o lab1

glad.o: glad.c
	gcc glad.c -c

mainWindow.o: mainWindow.cpp
	g++ mainWindow.cpp -c

clean:
	rm *.o lab1