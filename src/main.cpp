#include <iostream>
#include "../include/app.hpp"

int windowWidth = 1000;
int windowHeight = 1000;

int main()
{
	std::cout << "Hello World! \n" ;
	App app;
	app.Run(windowWidth, windowHeight);
}
