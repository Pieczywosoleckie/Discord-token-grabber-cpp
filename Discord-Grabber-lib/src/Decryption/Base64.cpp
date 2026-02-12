#include <Decryption.h>
#include <windows.h>
#include <wincrypt.h>
#include <vector>
#include <string>

#pragma comment(lib, "Crypt32.lib")



namespace Decryption
{

std::vector<BYTE> base64Decode(const std::string& input)
{
    DWORD size = 0;

    CryptStringToBinaryA(
        input.c_str(),
        0,
        CRYPT_STRING_BASE64,
        nullptr,
        &size,
        nullptr,
        nullptr
    );

    std::vector<BYTE> decoded(size);
    CryptStringToBinaryA(
        input.c_str(),
        0,
        CRYPT_STRING_BASE64,
        decoded.data(),
        &size,
        nullptr,
        nullptr
    );

    return decoded;
}

} // namespace Decryption

