/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Kyungwook Tak <k.tak@samsung.com>
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
 *
 * @file        test-key-provider.cpp
 * @author      Kyungwook Tak (k.tak@samsung.com)
 * @version
 * @brief
 */
#define BOOST_TEST_MODULE KEY_MANAGER_TEST
#include <boost/test/unit_test.hpp>
#include <exception.h>
#include <key-provider.h>
#include <test_common.h>
#include <iostream>

const CKM::Password PASSWORD = "12345TIZEN12345AAAAAAAAA";
const CKM::Password INCORRECT_PASSWORD = "AAAAAAAAAAAAAAAAAAAAA";
const CKM::Password NEW_PASSWORD = "NEW12345TIZEN12345NEW";

const std::string USERNAME_SHORT = "AB";
const std::string USERNAME_LONG = "SOFTWARE_CENTER_SYSTEM_SW_LAB_SECURITY_PART";
const std::string SMACK_LABEL_1 = "SAMPLE_SMACK_LABEL_1";
const std::string SMACK_LABEL_2 = "SAMPLE_SMACK_LABEL_2";

extern bool isLibInitialized;

BOOST_AUTO_TEST_SUITE(KEY_PROVIDER_TEST)
BOOST_AUTO_TEST_CASE(KeyDomainKEK){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_MESSAGE(keyProvider.isInitialized(),
            "KeyProvider created, but uninitialized");
}

BOOST_AUTO_TEST_CASE(KeyDomainKekInvalidPassword){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_THROW(keyProvider = CKM::KeyProvider(rb_test, INCORRECT_PASSWORD),
            CKM::Exc::AuthenticationFailed);
    BOOST_REQUIRE_MESSAGE(!keyProvider.isInitialized(),
            "KeyProvider not created, but initialized");
}

BOOST_AUTO_TEST_CASE(KeygetPureDomainKEK){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_MESSAGE(keyProvider.isInitialized(),
            "KeyProvider created, but uninitialized");
    BOOST_REQUIRE_NO_THROW(rb_test = keyProvider.getPureDomainKEK());
}

BOOST_AUTO_TEST_CASE(KeyGetWrappedDomainKEK){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_MESSAGE(keyProvider.isInitialized(),
            "KeyProvider created, but uninitialized");
    BOOST_REQUIRE_NO_THROW(rb_test = keyProvider.getWrappedDomainKEK(PASSWORD));
}

BOOST_AUTO_TEST_CASE(KeyGenerateDEK){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_test;
    CKM::RawBuffer rb_DEK1;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_MESSAGE(keyProvider.isInitialized(),
            "KeyProvider created, but uninitialized");
    BOOST_REQUIRE_NO_THROW(rb_DEK1 = keyProvider.generateDEK(SMACK_LABEL_1));
}

BOOST_AUTO_TEST_CASE(KeyGetPureDEK){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_pureDEK1;
    CKM::RawBuffer rb_DEK1;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_MESSAGE(keyProvider.isInitialized(),
            "KeyProvider created, but uninitialized");
    BOOST_REQUIRE_NO_THROW(rb_DEK1 = keyProvider.generateDEK(SMACK_LABEL_1));
    BOOST_REQUIRE_NO_THROW(rb_pureDEK1 = keyProvider.getPureDEK(rb_DEK1));
}

BOOST_AUTO_TEST_CASE(KeyReencrypt){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(CKM::KeyProvider::reencrypt(rb_test, PASSWORD,
            NEW_PASSWORD));
}

BOOST_AUTO_TEST_CASE(KeyReencrypt_incorrect_password){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_THROW((rb_test = CKM::KeyProvider::reencrypt(rb_test, INCORRECT_PASSWORD,
            NEW_PASSWORD)), CKM::Exc::AuthenticationFailed);
}

BOOST_AUTO_TEST_CASE(KeyGetPureDEK_after_reencrypt){
    BOOST_REQUIRE_MESSAGE(isLibInitialized,
            "Library is not initialized!");
    CKM::KeyProvider keyProvider;
    CKM::RawBuffer rb_DEK1;
    CKM::RawBuffer rb_test;
    BOOST_REQUIRE_NO_THROW(rb_test =
            CKM::KeyProvider::generateDomainKEK(USERNAME_LONG, PASSWORD));
    BOOST_REQUIRE_NO_THROW(keyProvider = CKM::KeyProvider(rb_test, PASSWORD));
    BOOST_REQUIRE_NO_THROW(rb_DEK1 = keyProvider.generateDEK(SMACK_LABEL_1));
    BOOST_REQUIRE_NO_THROW(keyProvider.getPureDEK(rb_DEK1));
}

BOOST_AUTO_TEST_SUITE_END()

