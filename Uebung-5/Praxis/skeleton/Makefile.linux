# // MAKEFILE for Computer Graphics Raytracer Exercise
EXECUTABLE = cg1_ex5
CC = g++
CPPFLAGS = -O3 -Igl -Iglm
#-I/usr/local/include/ -I/usr/X11/include/ -I/opt/local/include/
DEPS =  
OBJ = main.o
LIBS = -L/usr/local/lib/ -L/usr/X11/lib/ -L/opt/local/lib/  -lm -lstdc++ -lglut -lGL -lGLU
#-framework OpenGL 
#-framework GLUT -L/usr/local/lib/ -lglew -lglut

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)		
