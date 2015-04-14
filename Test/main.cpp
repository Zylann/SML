#include <iostream>
#include <fstream>
#include <sstream>

#include <sml/sml.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World!" << std::endl;

	const char * filename = "test_data/test.sml";
	std::ifstream ifs(filename, std::ios::in|std::ios::binary);

	if (ifs)
	{
		sml::Value v;
		v.loadFromStream(ifs);

		sml::Writer w;
		std::stringstream ss;

		w.writeValue(std::cout, v);
	}
	else
	{
		std::cout << "Unable to read " << filename << std::endl;
	}

	std::cout << "End" << std::endl;
	getchar();
	return 0;
}

