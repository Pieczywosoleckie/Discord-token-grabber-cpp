#include <Decryption.h>


namespace Decryption
{

std::vector<BYTE> decrypt_dpapi(const std::vector<BYTE>& encrypted) {
    DATA_BLOB in;
    in.pbData = const_cast<BYTE*>(encrypted.data());
    in.cbData = (DWORD)encrypted.size();

    DATA_BLOB out;

    if (!CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out)) {

        return {};
    }

    std::vector<BYTE> decrypted(out.pbData, out.pbData + out.cbData);
    LocalFree(out.pbData);
    return decrypted;
}

} // namespace Decryption