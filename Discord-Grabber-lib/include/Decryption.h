#pragma once
#include <vector>
#include <string>
#include <windows.h>

namespace Decryption
{

    std::vector<BYTE> base64Decode(const std::string& input);

    std::vector<BYTE> decrypt_dpapi(const std::vector<BYTE>& encrypted);

    std::string aes_gcm_decrypt(
        const std::vector<BYTE>& key,
        const std::vector<BYTE>& nonce,
        const std::vector<BYTE>& ciphertext,
        const std::vector<BYTE>& tag);

}