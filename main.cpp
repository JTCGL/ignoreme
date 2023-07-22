#include <iostream>
#include "added.h"
#include "glm\glm.hpp"

int main() {
	glm::vec3 myvec(1.2f, 3.4f, 5.6);
	std::cout << "Edited from cxxdroid, VS studio and VS code"<< std::endl;
	printf("sdasda, %f %f %f\n",myvec.x, myvec.y, myvec.z);
	added();
	return 0;
}
