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
#include "added.h"
#include <pets.hpp>

using namespace std;
using namespace glm;

int main() {

/*
What OS/compiler is this for platform-specific info ?
https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32#26225829
*/

#if defined(_WINDOWS)
	#if defined(_WIN64)
		cout << "std::string size:(win64)" << sizeof(string) << endl;
	#else
	#if defined(_WIN32)
		cout << "std::string size:(win32)" << sizeof(string) << endl;
	#endif
	#endif
#else
	//unknown os
	cout << "std::string size:(unknown system)" << sizeof(string) << endl;
#endif
	cat mycat("spot", 7);
	cat mycat2;
	mycat.speak();
	mycat2.speak();
	cout << "cat bounds (min x): " << mycat.dim().min.x << endl;
	vector<cat>mycats(5);
	for (auto &c : mycats) {
		cout << "This is a cat named: " << c.thiscat.name << endl;
	}
	vec3 myvec(1.2f, 3.4f, 5.6);
	cout << "Edited from cxxdroid, VS studio and VS code"<< std::endl;
	printf("sdasda, %f %f %f\n",myvec.x, myvec.y, myvec.z);
	added();

	//forget cats, we want a generc collection of animals - pets - of any type
	const uint8_t numpets = 4;
	vector<shared_ptr<animal>>pets(numpets);
	for (auto p = 0; p < numpets; p++) {
		if(p != numpets-1)
			pets[p] = make_shared<cat>("boots",p);
		else
			pets[p] = make_shared<dog>("sparky", p);
	}
	for (auto& pet : pets)
	{
		pet->speak();
	}

	return 0;
}
