all:
	g++ -o simulator src/*.cpp src/glad.c -Iinclude -lglfw -lGL 
