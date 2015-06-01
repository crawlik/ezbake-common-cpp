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

#ifndef EZBAKE_EZCONFIGURATION_HELPERS_SECURITYCONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_HELPERS_SECURITYCONFIGURATION_H_

#include <boost/shared_ptr.hpp>
#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "ezbake/configuration/helpers/BaseConfiguration.h"
#include "ezbake/configuration/helpers/ApplicationConfiguration.h"

namespace ezbake { namespace configuration { namespace helpers {

class SecurityConfiguration: public BaseConfiguration {
public:
    static const ::std::string DEFAULT_SSL_PROTOCOLS;
    static const ::std::string DEFAULT_SSL_CIPHERS;
    static const ::std::string DEFAULT_TURSTED_SSL_CERTS;
    static const ::std::string DEFAULT_SSL_CERT;
    static const ::std::string DEFAULT_PRIVATE_KEY;
    static const ::std::string DEFAULT_SERVICE_PUBLIC_KEY;

public:
    class UserCacheType {
        friend class SecurityConfiguration;

    public:
        static UserCacheType valueOf(const ::std::string& val);

        virtual ~UserCacheType();

        const ::std::string& toString() const;
        bool operator==(const UserCacheType& rhs) const;
        bool operator!=(const UserCacheType& rhs) const;

    private:
        UserCacheType(const ::std::string& name);

        ::std::string _name;
    };

    friend class UserCacheType;

public:
    static const UserCacheType MEMORY;
    static const UserCacheType REDIS;

public:
    static boost::shared_ptr<SecurityConfiguration> fromConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix = "");

    static ::std::string parseServiceKey(const ::std::string& key, const ::std::string& serviceName);

    virtual ~SecurityConfiguration();

    bool isPeerAuthRequired() const;
    bool useForwardProxy() const;
    UserCacheType getCacheType() const;
    long getUserCacheTTL() const;
    int getUserCacheSize() const;
    ::std::string getCertificatesDir() const;
    ::std::string getSecurityId() const;
    ::std::string getSslCiphers() const;
    ::std::string getSslProtocols() const;
    ::std::string trustedCert(const ::std::string& serviceName="") const;
    ::std::string privateKey(const ::std::string& serviceName="") const;
    ::std::string certificate(const ::std::string& serviceName="") const;
    ::std::string servicePublicKey(const ::std::string& serviceName="") const;
    ::std::string getPrivateKey(const ::std::string& serviceName="") const;
    ::std::string getTrustedSslCerts(const ::std::string& serviceName="") const;
    ::std::string getSslCertificate(const ::std::string& serviceName="") const;
    ::std::string getServicePublicKey(const ::std::string& serviceName="") const;
    bool hasPrivateKeySet() const;
    bool hasPublicKeySet() const;
    bool hasKeysSet() const;
    ::std::string sign(const ::std::string& serializedData) const;
    bool verify(const ::std::string& data, const ::std::string& base64Signature) const;

    void setPrivateKey(const ::std::string& serviceName="");
    void setPublicKey(const ::std::string& serviceName="");
    void setKeys();

private:
    ::std::string generateDigest(const ::std::string& data) const;

    SecurityConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix);

private:
    static const EVP_MD *_ALGORITHM;

    ::boost::shared_ptr<RSA> _publicKey;
    ::boost::shared_ptr<RSA> _privateKey;
    ::boost::shared_ptr<ApplicationConfiguration> _appConfig;
};

}}} // namespace ::ezbake::configuration::helpers

#endif /* EZBAKE_EZCONFIGURATION_HELPERS_SECURITYCONFIGURATION_H_ */
