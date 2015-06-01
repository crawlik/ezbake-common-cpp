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


#include "AllTests.h"
#include <ezbake/configuration/helpers/SecurityConfiguration.h>
#include <ezbake/security/core/EzSecurityTokenUtils.h>
#include <ezbake/security/client/CommonClient.h>
#include <ezbake/security/core/CommonUtils.h>

using namespace ::ezbake::security::core;
using namespace ::ezbake::security::client;
using namespace ::ezbake::configuration::helpers;
using namespace ::ezbake::base::thrift;
using namespace ::ezbake::security::core;

using ::ezbake::security::thrift::ProxyTokenRequest;

class EzSecurityTokenUtilsTest : public ::testing::Test {
public:
    EzSecurityTokenUtilsTest() {}
    virtual ~EzSecurityTokenUtilsTest() {}

    TokenRequest generateTokenRequest(const ::std::string& formattedDN, const ::std::string& securityId) {
        EzSecurityPrincipal principal;
        principal.principal = formattedDN;
        principal.validity.issuer = securityId;
        principal.validity.issuedTo = "__TEST_APP__";
        principal.validity.notAfter = CommonUtils::currentTimeMillis() + CommonClient::PRINCIPAL_EXPIRY;
        principal.validity.signature = "";

        TokenRequest request;
        request.__set_securityId(securityId);
        request.__set_timestamp(CommonUtils::currentTimeMillis());
        request.__set_principal(principal);
        request.__set_type(::ezbake::base::thrift::TokenType::USER);

        return request;
    }


    ProxyTokenRequest generateProxyTokenRequest(const ::std::string& formattDN,
            const ::std::string& securityId, const SecurityConfiguration& securityConfig) {

        X509Info x509Info;
        x509Info.__set_subject(formattDN);
        x509Info.__set_issuer(securityId);

        ValidityCaveats validityCaveats;
        validityCaveats.__set_issuer(securityId);
        validityCaveats.__set_issuedTo("");
        validityCaveats.__set_notAfter(CommonUtils::currentTimeMillis() +
                                       CommonClient::PRINCIPAL_EXPIRY);
        validityCaveats.__set_signature("");

        ::ezbake::security::thrift::ProxyTokenRequest request;
        request.__set_x509(x509Info);
        request.__set_validity(validityCaveats);

        //update signature
        request.validity.signature = EzSecurityTokenUtils::proxyTokenRequestSignature(request, securityConfig);

        return request;
    }
};


TEST_F(EzSecurityTokenUtilsTest, DeserializeProxyTokenFromJSON) {
    ProxyUserToken deserializedToken;
    ::std::string proxyTokenJSON =  ::std::string("") +
        "{\n"
            "\"x509\": {\n"
                "\"subject\": \"CN=John Snow, OU=NightsWatch, OU=Winterfell, O=HouseStark, C=GT\",\n"
                "\"issuer\": \"_TEST_APP_\"\n"
            "},\n"
            "\"issuedBy\": \"__TEST_ISSUER__\",\n"
            "\"issuedTo\": \"_Ez_APP\",\n"
            "\"notAfter\": 1407509507810\n"
        "}";

    try {
        deserializedToken = EzSecurityTokenUtils::deserializeProxyUserToken(proxyTokenJSON);
    } catch (const std::exception &e) {
        FAIL() << "Caught unexpected error with message: " << e.what();
    }

    EXPECT_FALSE(deserializedToken.__isset.notBefore);
    EXPECT_FALSE(deserializedToken.__isset.issuedFor);

    EXPECT_EQ("CN=John Snow, OU=NightsWatch, OU=Winterfell, O=HouseStark, C=GT",
            deserializedToken.x509.subject);
    EXPECT_TRUE(deserializedToken.x509.__isset.issuer);
    EXPECT_EQ("_TEST_APP_", deserializedToken.x509.issuer);
    EXPECT_EQ("__TEST_ISSUER__", deserializedToken.issuedBy);
    EXPECT_EQ("_Ez_APP", deserializedToken.issuedTo);
    EXPECT_EQ(1407509507810, deserializedToken.notAfter);
}


TEST_F(EzSecurityTokenUtilsTest, SerializeProxyTokenToJSON) {
    int64_t currentTime = CommonUtils::currentTimeMillis();

    X509Info x509;
    x509.__set_subject("CN=John Snow, OU=NightsWatch, OU=Winterfell, O=HouseStark, C=GT");
    x509.__set_issuer("__TEST_APP__");

    ProxyUserToken token;
    token.__set_x509(x509);
    token.__set_issuedBy("__TEST_ISSUER");
    token.__set_issuedTo("EzTestApp");
    token.__set_notBefore(currentTime - 18200);
    token.__set_notAfter(currentTime + 18200);

    std::string serializedToken = EzSecurityTokenUtils::serializeProxyUserTokenToJSON(token);
    ProxyUserToken roundTripped = EzSecurityTokenUtils::deserializeProxyUserToken(serializedToken);

    EXPECT_EQ(token, roundTripped);
}

