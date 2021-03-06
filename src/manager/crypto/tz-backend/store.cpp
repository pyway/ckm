/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */
/*
 * @file       store.cpp
 * @author     Bartłomiej Grzelewski (b.grzelewski@samsung.com)
 * @version    1.0
 */
#include <generic-backend/exception.h>
#include <tz-backend/obj.h>
#include <tz-backend/store.h>

namespace CKM {
namespace Crypto {
namespace TZ {

Store::Store(CryptoBackend backendId) :
    GStore(backendId)
{
}

GObjUPtr Store::getObject(const Token &, const  Password &)
{
    ThrowErr(Exc::Crypto::OperationNotSupported, "Trust zone backend is not implemented!");
}

TokenPair Store::generateAKey(const CryptoAlgorithm &, const Password &, const Password &)
{
    ThrowErr(Exc::Crypto::OperationNotSupported, "Trust zone backend is not implemented!");
}

Token Store::import(const Data &, const Password &)
{
    ThrowErr(Exc::Crypto::OperationNotSupported, "Trust zone backend is not implemented!");
}

Token Store::importEncrypted(const Data &, const Password &, const DataEncryption &)
{
    ThrowErr(Exc::Crypto::OperationNotSupported, "Trust zone backend is not implemented!");
}

} // namespace TZ
} // namespace Crypto
} // namespace CKM
