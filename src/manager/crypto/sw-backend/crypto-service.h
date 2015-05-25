#pragma once

#include <iostream>
#include <key-impl.h>
#include <certificate-impl.h>
#include <ckm/ckm-type.h>
#include <vector>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <dpl/exception.h>

#define DEV_HW_RANDOM_FILE    "/dev/hwrng"
#define DEV_URANDOM_FILE    "/dev/urandom"

#define EVP_SUCCESS 1	// DO NOTCHANGE THIS VALUE
#define EVP_FAIL    0	// DO NOTCHANGE THIS VALUE

#define CKM_CRYPTO_INIT_SUCCESS 1
#define CKM_CRYPTO_CREATEKEY_SUCCESS 2
#define CKM_VERIFY_CHAIN_SUCCESS 5
#define NOT_DEFINED -1

namespace CKM {
namespace Crypto {
namespace SW {

 // typedef std::vector<unsigned char> RawData; this must be defined in common header.
 // This is internal api so all functions should throw exception on errors.
class CryptoService {
public:
    CryptoService();
    virtual ~CryptoService();

    class Exception {
        public:
            DECLARE_EXCEPTION_TYPE(CKM::Exception, Base)
            DECLARE_EXCEPTION_TYPE(Base, Crypto_internal);
            DECLARE_EXCEPTION_TYPE(Base, opensslError);
    };

    static int createKeyPairRSA(const int size,      // size in bits [1024, 2048, 4096]
                        KeyImpl &createdPrivateKey,  // returned value ==> Key &createdPrivateKey,
                        KeyImpl &createdPublicKey);  // returned value ==> Key &createdPublicKey

    static int createKeyPairDSA(const int size,      // size in bits [1024, 2048, 3072, 4096]
                        KeyImpl &createdPrivateKey,  // returned value ==> Key &createdPrivateKey,
                        KeyImpl &createdPublicKey);  // returned value ==> Key &createdPublicKey

    static int createKeyPairECDSA(ElipticCurve type1,
                        KeyImpl &createdPrivateKey,  // returned value
                        KeyImpl &createdPublicKey);  // returned value

private:

};

} // namespace SW
} // namespace Crypto
} // namespace CKM
