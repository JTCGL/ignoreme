#include "glres.hpp"
#include <fstream>
#include <sstream>

std::string readtextfiletostring(const std::filesystem::path pathin) {
	std::string str;

	/*
	if (std::filesystem::exists(pathin)) {
		//pathin.generic_string()
		std::ifstream filein;/// (pathin);
		filein.open(pathin, std::ios::in | std::ios::binary);
		{

		}
	}
	*/

	/*
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		size_t sz = static_cast<size_t>(in.tellg());
		str.resize(sz);
		in.seekg(0, std::ios::beg);
		in.read(&str[0], str.size());
		in.close();
	}
	*/

	std::ifstream t(pathin);
	std::stringstream buffer;
	buffer << t.rdbuf();
	str = buffer.str();

	return str;
}

GLuint textfiletoshader(const std::filesystem::path pathin) {
	GLuint sh = 0;

	return 0;
}