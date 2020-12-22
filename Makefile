EXEC_FILE=test_file

FLAGS=-Wall
LIBS=-lglfw -ldl -Iglad
IN_PATH=-Iinclude
O_FILES= include/glad.o \
				 include/shader.o \
				 include/stb_image.o

all:	execute

# Compile GLAD
include/glad.o: include/glad.c
	g++ $^ -o $@ -c $(IN_PATH) $(FLAGS)

# Compile shaders
include/shader.o:	include/shader.cpp
	g++ $^ -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile image loader
include/stb_image.o: include/stb_image.cpp
	g++ $^ -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile main and link
$(EXEC_FILE):	main.cpp $(O_FILES)
	g++ $< $(O_FILES) -o $@ $(IN_PATH) $(LIBS) $(FLAGS)

execute:	$(EXEC_FILE)
	./$<

clean:
	rm include/*.o *.o $(EXEC_FILE) -f
