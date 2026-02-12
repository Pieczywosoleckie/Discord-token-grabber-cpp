#include <Decryption.h>

#include <windows.h>
#include <bcrypt.h>
#include <vector>
#include <string>
#include <stdexcept>

#pragma comment(lib, "bcrypt.lib")

namespace Decryption
{

std::string aes_gcm_decrypt(
    const std::vector<BYTE>& key,
    const std::vector<BYTE>& nonce,
    const std::vector<BYTE>& ciphertext,
    const std::vector<BYTE>& tag)
{
    BCRYPT_ALG_HANDLE hAlg = nullptr;
    BCRYPT_KEY_HANDLE hKey = nullptr;
    NTSTATUS status;

    status = BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_AES_ALGORITHM,
        nullptr,
        0
    );
    if (!BCRYPT_SUCCESS(status))
        throw std::runtime_error("Error");

    status = BCryptSetProperty(
        hAlg,
        BCRYPT_CHAINING_MODE,
        (PUCHAR)BCRYPT_CHAIN_MODE_GCM,
        sizeof(BCRYPT_CHAIN_MODE_GCM),
        0
    );
    if (!BCRYPT_SUCCESS(status))
        throw std::runtime_error("Error");

    status = BCryptGenerateSymmetricKey(
        hAlg,
        &hKey,
        nullptr,
        0,
        (PUCHAR)key.data(),
        (ULONG)key.size(),
        0
    );
    if (!BCRYPT_SUCCESS(status))
        throw std::runtime_error("Error");

    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);

    authInfo.pbNonce = (PUCHAR)nonce.data();
    authInfo.cbNonce = (ULONG)nonce.size();
    authInfo.pbTag = (PUCHAR)tag.data();
    authInfo.cbTag = (ULONG)tag.size();

    std::vector<BYTE> plaintext(ciphertext.size());
    ULONG size = 0;

    status = BCryptDecrypt(
        hKey,
        (PUCHAR)ciphertext.data(),
        (ULONG)ciphertext.size(),
        &authInfo,
        nullptr,
        0,
        plaintext.data(),
        (ULONG)plaintext.size(),
        &size,
        0
    );

    if (!BCRYPT_SUCCESS(status))
    {		
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
		throw std::runtime_error("Error");
	}

    plaintext.resize(size);

    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAlg, 0);

    return std::string(
        reinterpret_cast<char*>(plaintext.data()),
        plaintext.size()
    );
}

} // namespace Decryption

