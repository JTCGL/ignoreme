
#include <animal.h>

class cat : public animal {
	public:
	cat() { std::printf("%s\n", __func__); }
	cat(const std::string namein, const 	uint16_t agein) {
		thiscat.name = namein;
		thiscat.age = agein;
		printf("%s\n", __func__);
	}
	~cat() { printf("%s\n", __func__); }

	//copy/move? Want a collection of cats - pets - so be sure to only be checking the original!

	void speak() {
		std::cout << "I am a cat! my name is " << thiscat.name << " and I am " << thiscat.age << std::endl;
	}
	bbox dim() {
		return catbounds;
	}
	bbox catbounds{ glm::vec3{-1.f},glm::vec3{1.f} };
	id thiscat{ "default_cat", 2 };
};
