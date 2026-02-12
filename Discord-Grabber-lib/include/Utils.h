#pragma once 

#include <string>
#include <fstream>
#include <regex>

namespace Utils {
	bool ends_with(const std::string& str, const std::string& suffix);

	std::string GetEncryptKey(const std::string& path);


}