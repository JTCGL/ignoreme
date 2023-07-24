#pragma once
#ifndef ANIMAL_H
#define ANIMAL_H
//STL and common libs
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
//misc - some explicity included as this is meant to be cross platform
#include <cassert>
#include <cstdio> //printf
#include <cinttypes> //not just the types in cstdint, but the printf macros for them too
//specific to this
#include <glm.hpp>

struct bbox {
	glm::vec3 min;
	glm::vec3 max;
	//overload '<<' operator ?
};
struct id {
	std::string name;
	uint16_t age;
};

class animal {
public:
	animal() {}
	virtual ~animal() { std::cout << "animal(dtor)\n"; };
	virtual void speak() = 0;
	virtual bbox dim() = 0;
};
#endif
