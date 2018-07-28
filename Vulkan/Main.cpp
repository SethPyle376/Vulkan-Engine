#include "Renderer.h"

int main()
{
	Renderer app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		system("PAUSE");
		return EXIT_FAILURE;
	}
	system("PAUSE");
	return EXIT_SUCCESS;
}
