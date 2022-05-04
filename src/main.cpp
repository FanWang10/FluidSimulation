#include "Window.hpp"

int main()
{
	// INitialize a window
	int width = 800;
	int height = 600;
	Window window(width, height, "FluidSimulation");
	window.OnExecute();

}
