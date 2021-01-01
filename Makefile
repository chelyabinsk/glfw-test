EXEC_FILE=test_file

FLAGS=-Wall -g
LIBS=-lglfw -ldl -Iglad -lao -lmpg123 -lfftw3f -lm
IN_PATH=-Iinclude
O_FILES= include/glad/glad.o \
         include/shader.o \
         include/stb_image.o \
         include/plot/bar.o \
         include/plot/circlePlot.o \
         include/plot/linePlot.o \
         include/plot/surface.o \
         include/shapes/square.o
         
all:	execute

# Compile static sound player
play: play.c
	g++ -O2 -o play play.c -lao -lmpg123 -Iinclude -lfftw3f -lm

# Compile GLAD
include/glad/glad.o: include/glad/glad.c include/glad/gl.h
	g++ $< -o $@ -c $(IN_PATH) $(FLAGS)

# Compile shaders
include/shader.o: include/shader.cpp include/shader.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile image loader
include/stb_image.o: include/stb_image.cpp include/stb_image.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)
	
# Compile bar
include/plot/bar.o: include/plot/bar.cpp include/plot/bar.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile cirlePlot
include/plot/circlePlot.o: include/plot/circlePlot.cpp include/plot/circlePlot.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile linePlot
include/plot/linePlot.o: include/plot/linePlot.cpp include/plot/linePlot.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)
	
# Compile Surface
include/plot/surface.o: include/plot/surface.cpp include/plot/surface.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)
	
# Compile Square
include/shapes/square.o: include/shapes/square.cpp include/shapes/square.h
	g++ $< -o $@ -c $(IN_PATH) $(LIBS) $(FLAGS)

# Compile main and link
$(EXEC_FILE): main.cpp $(O_FILES)
	g++ $< $(O_FILES) -o $@ $(IN_PATH) $(LIBS) $(FLAGS)

execute: $(EXEC_FILE)
	./$<
	
clean_mpg123:
	rm include/mpg123/src/*.o include/mpg123/src/mpg123 include/mpg123/src/mpg123-id3dump include/mpg123/src/mpg123-strip include/mpg123/src/mpg123-with-modules include/mpg123/libtool -rf
	
clean: clean_mpg123	
	rm include/*.o *.o $(EXEC_FILE) include/plot/*.o -f
