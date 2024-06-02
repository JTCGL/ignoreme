#pragma once
#include "platform.hpp"
#include <filesystem>

class glres {
public:
	virtual const char* Name() const = 0;
	virtual GLuint Id() const = 0;
	virtual ~glres() {};
};

extern std::string readtextfiletostring(const std::filesystem::path pathin);
extern GLuint textfiletoshader(const std::filesystem::path pathin);