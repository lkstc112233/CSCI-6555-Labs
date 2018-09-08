LINK_FLAGS = -L/System/Library/Frameworks -framework GLUT -framework OpenGL

lab1: mainWindow.o
	g++ mainWindow.o $(LINK_FLAGS) -o lab1

mainWindow.o: mainWindow.cpp
	g++ mainWindow.cpp -c -o mainWindow.o

clean:
	rm *.o lab1