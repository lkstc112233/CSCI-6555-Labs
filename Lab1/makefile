LINK_FLAGS = -lglfw

lab1: glad.o mainWindow.o fileLoader.o
	g++-8 glad.o mainWindow.o fileLoader.o $(LINK_FLAGS) -o lab1

glad.o: glad.c
	gcc glad.c -c

mainWindow.o: mainWindow.cpp fileLoader.h
	g++-8 mainWindow.cpp -c

fileLoader.o: fileLoader.cpp fileLoader.h
	g++-8 fileLoader.cpp -c

clean:
	find . -type f -name '*.o' -delete
	rm lab1