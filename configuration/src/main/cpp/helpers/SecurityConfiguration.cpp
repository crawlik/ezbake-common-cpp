/*   Copyright (C) 2013-2015 Computer Sciences Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "ezbake/configuration/helpers/SecurityConfiguration.h"

#include <stdexcept>

#include <openssl/pem.h>

#include "EzBakeProperty_constants.h"
#include "ezbake/configuration/helpers/SystemConfiguration.h"
#include "ezbake/configuration/util/CryptoUtil.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

const ::std::string SecurityConfiguration::DEFAULT_SSL_PROTOCOLS = "TLSv1";
const ::std::string SecurityConfiguration::DEFAULT_SSL_CIPHERS = "HIGH:!DSS:!aNULL@STRENGTH";
const ::std::string SecurityConfiguration::DEFAULT_TURSTED_SSL_CERTS = "ezbakeca.crt";
const ::std::string SecurityConfiguration::DEFAULT_SSL_CERT = "application.crt";
const ::std::string SecurityConfiguration::DEFAULT_PRIVATE_KEY = "application.priv";
const ::std::string SecurityConfiguration::DEFAULT_SERVICE_PUBLIC_KEY = "ezbakesecurityservice.pub";
const SecurityConfiguration::UserCacheType SecurityConfiguration::MEMORY = SecurityConfiguration::UserCacheType("MEMORY");
const SecurityConfiguration::UserCacheType SecurityConfiguration::REDIS = SecurityConfiguration::UserCacheType("REDIS");
const EVP_MD* SecurityConfiguration::_ALGORITHM = EVP_sha256();

SecurityConfiguration::UserCacheType
SecurityConfiguration::UserCacheType::valueOf(const ::std::string& val) {
    SecurityConfiguration::UserCacheType mode = MEMORY;
    if (REDIS._name == val) {
        mode = REDIS;
    } else if (MEMORY._name != val) {
        BOOST_THROW_EXCEPTION(std::invalid_argument(val + " is not a supported user cache mode"));
    }
    return mode;
}

SecurityConfiguration::UserCacheType::UserCacheType(const ::std::string& name) : _name(name) {
}

SecurityConfiguration::UserCacheType::~UserCacheType() {
}

const ::std::string& SecurityConfiguration::UserCacheType::toString() const {
    return _name;
}

bool SecurityConfiguration::UserCacheType::operator==(const SecurityConfiguration::UserCacheType& rhs) const {
    return (_name == rhs._name);
}

bool SecurityConfiguration::UserCacheType::operator!=(const SecurityConfiguration::UserCacheType& rhs) const {
    return !(*this == rhs);
}

::std::string SecurityConfiguration::parseServiceKey(
        const ::std::string& key, const ::std::string& serviceName) {
    if (::std::string::npos != serviceName.find_first_not_of(' ')) {
        return key + "." + serviceName;
    }

    return key;
}

boost::shared_ptr<SecurityConfiguration> SecurityConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    boost::shared_ptr<SecurityConfiguration> p(
        new SecurityConfiguration(configuration, namespacePrefix));

    return p;
}

SecurityConfiguration::SecurityConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix),
      _appConfig(ApplicationConfiguration::fromConfiguration(configuration, namespacePrefix)) {
}

SecurityConfiguration::~SecurityConfiguration() {
}

bool SecurityConfiguration::isPeerAuthRequired() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.EZBAKE_SSL_PEER_AUTH_REQUIRED),
        true);
}

bool SecurityConfiguration::useForwardProxy() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.EZBAKE_USE_FORWARD_PROXY),
        false);
}

SecurityConfiguration::UserCacheType SecurityConfiguration::getCacheType() const {
    ::std::string config = ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_USER_CACHE_TYPE),
        MEMORY.toString());

    return UserCacheType::valueOf(config);
}

long SecurityConfiguration::getUserCacheTTL() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.EZBAKE_USER_CACHE_TTL),
        43200);
}

int SecurityConfiguration::getUserCacheSize() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.EZBAKE_USER_CACHE_SIZE),
        1000);
}

::std::string SecurityConfiguration::getCertificatesDir() const {
    return _appConfig->getCertificatesDir();
}

::std::string SecurityConfiguration::getSecurityId() const {
    return _appConfig->getSecurityID();
}

::std::string SecurityConfiguration::getSslCiphers() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_SSL_CIPHERS_KEY),
        DEFAULT_SSL_CIPHERS);
}

::std::string SecurityConfiguration::getSslProtocols() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_SSL_PROTOCOL_KEY),
        DEFAULT_SSL_PROTOCOLS);
}

::std::string SecurityConfiguration::trustedCert(const ::std::string& serviceName) const {
    return ezConfiguration.getString(
            parseServiceKey(
                parseKey(
                    g_EzBakeProperty_constants.EZBAKE_APPLICATION_TRUSTED_CERT),
                    serviceName),
            DEFAULT_TURSTED_SSL_CERTS);
}

::std::string SecurityConfiguration::privateKey(const ::std::string& serviceName) const {
    return ezConfiguration.getString(
            parseServiceKey(
                parseKey(
                    g_EzBakeProperty_constants.EZBAKE_APPLICATION_PRIVATE_KEY_FILE),
                    serviceName),
            DEFAULT_PRIVATE_KEY);
}

::std::string SecurityConfiguration::certificate(const ::std::string& serviceName) const {
    return ezConfiguration.getString(
            parseServiceKey(
                parseKey(
                    g_EzBakeProperty_constants.EZBAKE_APPLICATION_CERT_FILE),
                    serviceName),
            DEFAULT_SSL_CERT);
}

::std::string SecurityConfiguration::servicePublicKey(const ::std::string& serviceName) const {
    return ezConfiguration.getString(
            parseServiceKey(
                parseKey(
                    g_EzBakeProperty_constants.EZBAKE_APPLICATION_PUBLIC_KEY_FILE),
                    serviceName),
            DEFAULT_SERVICE_PUBLIC_KEY);
}

::std::string SecurityConfiguration::getPrivateKey(const ::std::string& serviceName) const {
    return getCertificatesDir() + SystemConfiguration::FILE_SEPARATOR + privateKey(serviceName);
}

::std::string SecurityConfiguration::getTrustedSslCerts(const ::std::string& serviceName) const {
    return getCertificatesDir() + SystemConfiguration::FILE_SEPARATOR + trustedCert(serviceName);
}

::std::string SecurityConfiguration::getSslCertificate(const ::std::string& serviceName) const {
    return getCertificatesDir() + SystemConfiguration::FILE_SEPARATOR + certificate(serviceName);
}

::std::string SecurityConfiguration::getServicePublicKey(const ::std::string& serviceName) const {
    return getCertificatesDir() + SystemConfiguration::FILE_SEPARATOR + servicePublicKey(serviceName);
}

bool SecurityConfiguration::hasPrivateKeySet() const {
    return (_privateKey.get() != NULL);
}

bool SecurityConfiguration::hasPublicKeySet() const {
    return (_publicKey.get() != NULL);
}

bool SecurityConfiguration::hasKeysSet() const {
    return (hasPrivateKeySet() && hasPublicKeySet());
}

::std::string SecurityConfiguration::sign(const ::std::string& serializedData) const {
    if (!hasPrivateKeySet()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Cannot sign data. Private key has not been set. Did you call setPrivateKey/setKeys?"));
    }

    try {
        ::std::string digest = generateDigest(serializedData);

        unsigned int rawSigLength = 0;
        unsigned char *rawSig = new unsigned char[RSA_size(_privateKey.get())];

        RSA_sign(
            EVP_MD_type(_ALGORITHM),
            reinterpret_cast<unsigned char*>(const_cast<char*>(digest.c_str())),
            static_cast<unsigned int>(digest.length()),
            rawSig,
            &rawSigLength,
            _privateKey.get());

        ::std::string signature = ::ezbake::configuration::util::CryptoUtil::encode(
            reinterpret_cast<const char*>(rawSig), rawSigLength);

        delete[] rawSig;
        return signature;
    } catch (std::exception &e) {
        BOOST_THROW_EXCEPTION(std::runtime_error(::std::string("Error in signing data: ") + e.what()));
    }

    return "";
}

bool SecurityConfiguration::verify(const ::std::string& data, const ::std::string& base64Signature) const {
    if (!hasPublicKeySet()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Cannot verify data. Public key has not been set. Did you call setPublicKey/setKeys?"));
    }

    try {
        ::std::string digest = generateDigest(data);
        ::std::string signature = ::ezbake::configuration::util::CryptoUtil::decode(base64Signature);

        return RSA_verify(
            EVP_MD_type(_ALGORITHM),
            reinterpret_cast<unsigned char*>(const_cast<char *>(digest.c_str())),
            static_cast<unsigned int>(digest.length()),
            reinterpret_cast<unsigned char*>(const_cast<char *>(signature.data())),
            static_cast<unsigned int>(signature.length()),
            _publicKey.get());

    } catch (std::exception &e) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            ::std::string("Error in verifying data: ") + e.what()));
    }

    return false;
}

void SecurityConfiguration::setPrivateKey(const ::std::string& serviceName) {
    FILE * pk = fopen(getPrivateKey(serviceName).c_str(), "rb");
    if (NULL == pk) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Error in opening private key file"));
    }

    _privateKey.reset(PEM_read_RSAPrivateKey(pk,NULL,NULL,NULL));
    fclose(pk);

    if (NULL == _privateKey.get()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Error in reading private key: PEM_read_RSAPrivateKey returned NULL"));
    }
}

void SecurityConfiguration::setPublicKey(const ::std::string& serviceName) {
    FILE * pk = fopen(getServicePublicKey(serviceName).c_str(), "rb");
    if (NULL == pk) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Error in opening public key file"));
    }

    _publicKey.reset(PEM_read_RSA_PUBKEY(pk,NULL,NULL,NULL));
    fclose(pk);

    if (NULL == _publicKey.get()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Error in reading public key: PEM_read_RSA_PUBKEY returned NULL"));
    }
}

void SecurityConfiguration::setKeys() {
    FILE * pk = fopen(getPrivateKey().c_str(), "rb");
    if (NULL == pk) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Error in opening private key file"));
    }

    _privateKey.reset(PEM_read_RSAPrivateKey(pk,NULL,NULL,NULL));
    fclose(pk);

    pk = fopen(getServicePublicKey().c_str(), "rb");
    if (NULL == pk) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Error in opening public key file"));
    }

    _publicKey.reset(PEM_read_RSA_PUBKEY(pk,NULL,NULL,NULL));
    fclose(pk);

    if (NULL == _privateKey.get()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Error in reading private key: PEM_read_RSAPrivateKey returned NULL"));
    }
    if (NULL == _publicKey.get()) {
        BOOST_THROW_EXCEPTION(std::runtime_error(
            "Error in reading public key: PEM_read_RSA_PUBKEY returned NULL"));
    }
}

::std::string SecurityConfiguration::generateDigest(const ::std::string& data) const {
    EVP_MD_CTX *ctx = EVP_MD_CTX_create();
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int mdLength = 0;
    EVP_add_digest(_ALGORITHM);
    if (!EVP_DigestInit_ex(ctx, _ALGORITHM, NULL) ||
        !EVP_DigestUpdate(ctx, data.data(), data.length()) ||
        !EVP_DigestFinal_ex(ctx, md, &mdLength)) {
        EVP_MD_CTX_destroy(ctx);
        BOOST_THROW_EXCEPTION(std::runtime_error("Error in generating digest"));
    }
    EVP_MD_CTX_destroy(ctx);

    return ::std::string(reinterpret_cast<char *>(md), mdLength);
}

}}} // namespace ::ezbake::configuration::helpers
