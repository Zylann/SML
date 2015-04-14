#include <iostream>
#include <fstream>
#include <sstream>

#include <sml/sml.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	std::ifstream ifs("test.sml", std::ios::in|std::ios::binary);

	if (ifs)
	{
		sml::Value v;
		v.loadFromStream(ifs);

		sml::Writer w;
		std::stringstream ss;

		w.writeValue(std::cout, v);
	}

	std::cout << "End" << std::endl;
	getchar();
	return 0;
}

