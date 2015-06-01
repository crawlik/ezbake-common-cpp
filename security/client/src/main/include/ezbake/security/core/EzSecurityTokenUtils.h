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

#ifndef EZBAKE_SECURITY_CORE_EZSECURITYTOKENUTILS_H_
#define EZBAKE_SECURITY_CORE_EZSECURITYTOKENUTILS_H_

#include <sstream>
#include <stdexcept>
#include "EzSecurity.h"
#include "ezbake/configuration/helpers/SecurityConfiguration.h"
#include "ezbake/security/core/CommonUtils.h"

namespace ezbake { namespace security { namespace core {

class EzSecurityTokenUtils {
public:
    static bool isEzAdmin(const ::ezbake::security::thrift::EzSecurityToken& token);

    static ::std::string tokenRequestSignature(const ::ezbake::security::thrift::TokenRequest& request,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyTokenRequestSignature(const ::ezbake::security::thrift::TokenRequest& request,
            const ::std::string& signature, const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static ::std::string serializeProxyTokenRequest(const ::ezbake::security::thrift::ProxyTokenRequest& request);

    static ::std::string proxyTokenRequestSignature(const ::ezbake::security::thrift::ProxyTokenRequest& request,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyProxyTokenRequest(const ::ezbake::security::thrift::ProxyTokenRequest& request,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static ::std::string proxyUserTokenSignature(const ::ezbake::base::thrift::ProxyUserToken& token,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyProxyUserToken(const ::std::string& token, const ::std::string& signature,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyProxyUserToken(const ::ezbake::base::thrift::ProxyUserToken& token, const ::std::string& signature,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static ::std::string serializeProxyUserTokenToJSON(const ::ezbake::base::thrift::ProxyUserToken& token);

    static ::ezbake::base::thrift::ProxyUserToken deserializeProxyUserToken(const ::std::string& token);

    static ::std::string tokenSignature(const ::ezbake::security::thrift::EzSecurityToken& token,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyTokenSignature(const ::ezbake::security::thrift::EzSecurityToken& token,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static bool verifyPrincipalSignature(const ::ezbake::security::thrift::EzSecurityPrincipal& principal,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static ::std::string serializeTokenRequest(const ::ezbake::security::thrift::TokenRequest& token);

    static ::std::string serializeProxyUserToken(const ::ezbake::base::thrift::ProxyUserToken &token);

    static ::std::string serializeToken(const ::ezbake::security::thrift::EzSecurityToken& token);

    static ::std::string principalSignature(const ::ezbake::security::thrift::EzSecurityPrincipal& principal,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static ::std::string serializePrincipal(const ::ezbake::security::thrift::EzSecurityPrincipal& principal);

    static bool verifyReceivedToken(const ::ezbake::security::thrift::EzSecurityToken& token, const ::std::string id,
            const ::ezbake::configuration::helpers::SecurityConfiguration& securityConfig);

    static int64_t getEzSecurityTokenJsonExpiration(const ::ezbake::base::thrift::EzSecurityTokenJson& token);

    inline static int64_t getProxyTokenResponseExpiration(const ::ezbake::security::thrift::ProxyTokenResponse& token) {
        return EzSecurityTokenUtils::deserializeProxyUserToken(token.token).notAfter;
    }

    inline static bool isEzSecurityTokenJsonExpired(const ::ezbake::base::thrift::EzSecurityTokenJson& jsonToken) {
        return !(getEzSecurityTokenJsonExpiration(jsonToken) >
                 ::ezbake::security::core::CommonUtils::currentTimeMillis());
    }

    inline static bool isProxyTokenResponseExpired(const ::ezbake::security::thrift::ProxyTokenResponse& responseToken) {
        return !(getProxyTokenResponseExpiration(responseToken) >
                 ::ezbake::security::core::CommonUtils::currentTimeMillis());
    }

    inline static ::std::string serializeTokenType(::ezbake::base::thrift::TokenType::type type) {
        ::std::string rtn;
        switch(type) {
        case ::ezbake::base::thrift::TokenType::USER:
            rtn = "USER";
            break;
        case ::ezbake::base::thrift::TokenType::APP:
            rtn = "APP";
            break;
        default:
            BOOST_THROW_EXCEPTION(::std::runtime_error("error in serializing TokenRequest. Unsupported TokenType"));
            break;
        }
        return rtn;
    }

    inline static ::std::string serializeBoolean(bool value) {
        if (value) {
            return "true";
        }
        return "false";
    }

private:
    EzSecurityTokenUtils() {}
};

}}} // namespace ezbake::security::core

#endif /* EZBAKE_SECURITY_CORE_EZSECURITYTOKENUTILS_H_ */
