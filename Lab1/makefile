LINK_FLAGS = -lglfw

lab1: glad.o mainWindow.o fileop/fileLoader.o
	g++-8 glad.o mainWindow.o fileop/fileLoader.o $(LINK_FLAGS) -o lab1

glad.o: glad.c
	gcc glad.c -c

mainWindow.o: mainWindow.cpp fileop/fileLoader.h
	g++-8 mainWindow.cpp -c

fileop/fileLoader.o: fileop/fileLoader.cpp fileop/fileLoader.h
	g++-8 fileop/fileLoader.cpp -c -o fileop/fileLoader.o

clean:
	find . -type f -name '*.o' -delete
	rm lab1