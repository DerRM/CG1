EXECUTABLE = cg1_ex4
CC = g++
CPPFLAGS = -g3 -Wall -Iglm-0.9.4.0 #exchanged O for g for debugging without optimization
DEPS = glm-0.9.4.0/glm/glm.hpp glm-0.9.4.0/glm/gtc/constants.inl Context.hpp Texture.hpp Image.hpp GLSLShader.h Mesh.h
OBJ = cg1_ex4.o Context.o Texture.o Image.o GLSLShader.o Mesh.o
LIBS = -lGLEW -lGLU -lGL -lglut

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
