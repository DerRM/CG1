EXECUTABLE = cg1_ex2
CC = g++
CPPFLAGS = -O3 -Iglm-0.9.3.4
DEPS = glm-0.9.3.4/glm/glm.hpp cell.h context.h window.h projection.h objmodel.h
OBJ = cell.o context.o window.o projection.o cg1_ex2.o objmodel.o
LIBS = -lGLU -lGL -lglut

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLES)		