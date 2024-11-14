#include "FileAction.h"

#include <iostream>
#include <cstring>
#include <string>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: " << argv[0] << " [-s | -c] <filename> [byte_count] [-d]" << std::endl;
		return -1;
	}
	std::string mode = argv[1];
	std::string filename = argv[2];
	bool delete_after = false;
	if (argc > 3 && std::strcmp(argv[3], "-d") == 0)
	   delete_after = true;
	
	FileAction obj(delete_after);

	try {
		if (mode == "-c") {
			obj.construct(filename);
		} else if (mode == "-s") {
			std::size_t byte_count = 10485760;
			obj.segment(filename, byte_count);
		} else {
			throw std::runtime_error("unknown mode");
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
