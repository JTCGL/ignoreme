
#include <animal.h>

class dog : public animal {
	public:
	dog() { std::printf("%s\n", __func__); }
	dog(const std::string namein, const 	uint16_t agein) {
		thisdog.name = namein;
		thisdog.age = agein;
		std::printf("%s\n", __func__);
	}
	~dog() { std::printf("%s\n", __func__); }

	//copy/move? Want a collection of cats - pets - so be sure to only be checking the original!

	void speak() {
		std::cout << "I am a dog! my name is " << thisdog.name << " and I am " << thisdog.age << std::endl;
	}
	bbox dim() {
		return dogbounds;
	}
	bbox dogbounds{ glm::vec3{-1.f},glm::vec3{1.f} };
	id thisdog{ "default_dog", 2 };
};
