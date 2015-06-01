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

#include <cinttypes>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log4cxx/logger.h>

#pragma GCC diagnostic ignored "-Wconversion"
#define PICOJSON_USE_INT64
#include "contrib/picojson/picojson.h"
#pragma GCC diagnostic warning "-Wconversion"

#include "ezbake/security/core/EzSecurityTokenUtils.h"
#include "ezbake/security/core/EzSecurityConstants.h"

namespace {
log4cxx::LoggerPtr const LOG = log4cxx::Logger::getLogger("ezbake.security.core.EzSecurityTokenUtils");
}

namespace ezbake { namespace security { namespace core {

using namespace ::ezbake::security::core;

using ::ezbake::configuration::helpers::SecurityConfiguration;
using ::ezbake::security::thrift::EzSecurityToken;
using ::ezbake::security::thrift::TokenRequest;
using ::ezbake::security::thrift::ProxyTokenRequest;
using ::ezbake::security::thrift::ProxyTokenResponse;
using ::ezbake::base::thrift::ProxyUserToken;
using ::ezbake::base::thrift::EzSecurityTokenJson;

bool EzSecurityTokenUtils::isEzAdmin(const EzSecurityToken& token) {
    bool admin = false;
    if (token.externalProjectGroups.size() > 0) {
        ::std::map< ::std::string, ::std::vector< ::std::string> >::const_iterator projectItr =
                token.externalProjectGroups.find(EzSecurityConstants::EZ_INTERNAL_PROJECT);
        if (projectItr != token.externalProjectGroups.end()) {
            admin = ::std::find(projectItr->second.begin(), projectItr->second.end(),
                                EzSecurityConstants::EZ_INTERNAL_ADMIN_GROUP) !=
                                        projectItr->second.end();
        }
    }

    return admin;
}

::std::string EzSecurityTokenUtils::tokenRequestSignature(const ::ezbake::security::thrift::TokenRequest& request,
        const SecurityConfiguration& securityConfig) {
    try {
        return securityConfig.sign(serializeTokenRequest(request));
    } catch (const ::std::exception& ex) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Unable to generate TokenRequest signature: " +
                ::boost::diagnostic_information(ex)));
    }
    return "";
}

bool EzSecurityTokenUtils::verifyTokenRequestSignature(const ::ezbake::security::thrift::TokenRequest& request,
        const ::std::string& signature, const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        verifies = securityConfig.verify(serializeTokenRequest(request), signature);
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

::std::string EzSecurityTokenUtils::serializeProxyTokenRequest( const ::ezbake::security::thrift::ProxyTokenRequest& request) {
    ::std::ostringstream ss;
    ss << request.x509.subject;
    if (request.x509.__isset.issuer) {
        ss << request.x509.issuer;
    }
    ss << request.validity.issuer
           << request.validity.issuedTo;
    if (request.validity.__isset.issuedFor) {
        ss << request.validity.issuedFor;
    }
    if (request.validity.__isset.issuedTime) {
        ss << request.validity.issuedTime;
    }
    if (request.validity.__isset.notBefore) {
        ss << request.validity.notBefore;
    }
    ss << request.validity.notAfter;
    return ss.str();
}

::std::string EzSecurityTokenUtils::proxyTokenRequestSignature(const ::ezbake::security::thrift::ProxyTokenRequest& request,
        const SecurityConfiguration& securityConfig) {
    try {
        return securityConfig.sign(serializeProxyTokenRequest(request));
    } catch (const ::std::exception& ex) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Unable to generate ProxyTokenRequest signature: " +
                ::boost::diagnostic_information(ex)));
    }
    return "";
}

bool EzSecurityTokenUtils::verifyProxyTokenRequest(const ::ezbake::security::thrift::ProxyTokenRequest& request,
        const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        verifies = securityConfig.verify(serializeProxyTokenRequest(request), request.validity.signature);
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

::std::string EzSecurityTokenUtils::proxyUserTokenSignature(const ::ezbake::base::thrift::ProxyUserToken& token,
        const SecurityConfiguration& securityConfig) {
    try {
        return securityConfig.sign(serializeProxyUserToken(token));
    } catch (const ::std::exception& ex) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Unable to generate ProxyUserToken signature: " +
                ::boost::diagnostic_information(ex)));
    }
    return "";
}

bool EzSecurityTokenUtils::verifyProxyUserToken(const ::std::string& token, const ::std::string& signature,
        const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        verifies = securityConfig.verify(token, signature);
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

bool EzSecurityTokenUtils::verifyProxyUserToken(const ::ezbake::base::thrift::ProxyUserToken& token,
        const ::std::string& signature, const SecurityConfiguration& securityConfig) {
    return verifyProxyUserToken(serializeProxyUserTokenToJSON(token), signature, securityConfig);
}

::std::string EzSecurityTokenUtils::serializeProxyUserTokenToJSON(const ::ezbake::base::thrift::ProxyUserToken& token) {
    picojson::object x509;
    x509["subject"] = picojson::value(token.x509.subject);
    if (token.x509.__isset.issuer) {
        x509["issuer"] = picojson::value(token.x509.issuer);
    }

    picojson::object token_obj;
    token_obj["x509"] = picojson::value(x509);
    token_obj["issuedBy"] = picojson::value(token.issuedBy);
    token_obj["issuedTo"] = picojson::value(token.issuedTo);
    token_obj["notAfter"] = picojson::value(token.notAfter);

    if (token.__isset.issuedFor) {
        token_obj["issuedFor"] = picojson::value(token.issuedFor);
    }

    if (token.__isset.notBefore) {
        token_obj["notBefore"] = picojson::value(token.notBefore);
    }

    picojson::value token_out{token_obj};

    return token_out.serialize();
}

::ezbake::base::thrift::ProxyUserToken EzSecurityTokenUtils::deserializeProxyUserToken(const ::std::string& tokenString) {
    ::ezbake::base::thrift::ProxyUserToken token;

    ::picojson::value token_json;
    ::std::string parse_error = ::picojson::parse(token_json, tokenString);
    if (!parse_error.empty()) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Exception in deserializing token: " + parse_error));
    }

    if (!token_json.is<picojson::object>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument("JSON is not an object"));
    }

    auto token_obj = token_json.get<picojson::object>();

    auto issuedBy = token_obj["issuedBy"];
    if (!issuedBy.is<std::string>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"issuedBy\" or the field is not a string"));
    }

    token.issuedBy = issuedBy.get<std::string>();

    auto issuedTo = token_obj["issuedTo"];
    if (!issuedTo.is<std::string>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"issuedTo\" or the field is not a string"));
    }

    token.issuedTo = issuedTo.get<std::string>();

    auto issuedFor = token_obj["issuedFor"];
    if (!issuedFor.is<picojson::null>()) {
        if (!issuedFor.is<std::string>()) {
            BOOST_THROW_EXCEPTION(::std::invalid_argument("JSON field \"issuedFor\" is not a string"));
        }

        token.__set_issuedFor(issuedFor.get<std::string>());
    }

    auto notAfter = token_obj["notAfter"];
    if (!notAfter.is<int64_t>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"notAfter\" or the field is not a number"));
    }

    token.notAfter = notAfter.get<int64_t>();

    auto notBefore = token_obj["notBefore"];
    if (!notBefore.is<picojson::null>()) {
        if (!notBefore.is<int64_t>()) {
            BOOST_THROW_EXCEPTION(::std::invalid_argument("JSON field \"notBefore\" is not a number"));
        }

        token.__set_notBefore(notBefore.get<int64_t>());
    }

    auto x509 = token_obj["x509"];
    if (!x509.is<picojson::object>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"x509\" or the field is not a JSON object"));
    }

    auto x509_obj = x509.get<picojson::object>();

    auto subject = x509_obj["subject"];
    if (!subject.is<std::string>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"x509.subject\" or the field is not a string"));
    }

    token.x509.subject = subject.get<std::string>();

    auto issuer = x509_obj["issuer"];
    if (!issuer.is<picojson::null>()) {
        if (!issuer.is<std::string>()) {
            BOOST_THROW_EXCEPTION(::std::invalid_argument("JSON field \"x509.issuer\" is not a string"));
        }

        token.x509.__set_issuer(issuer.get<std::string>());
    }

    return token;
}

::std::string EzSecurityTokenUtils::tokenSignature(const ::ezbake::security::thrift::EzSecurityToken& token,
        const SecurityConfiguration& securityConfig) {
    try {
        return securityConfig.sign(serializeToken(token));
    } catch (const ::std::exception& ex) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Unable to generate EzSecurityToken signature: " +
                ::boost::diagnostic_information(ex)));
    }
    return "";
}

bool EzSecurityTokenUtils::verifyTokenSignature(const ::ezbake::security::thrift::EzSecurityToken& token,
        const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        verifies = securityConfig.verify(serializeToken(token), token.validity.signature);
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

bool EzSecurityTokenUtils::verifyPrincipalSignature(const ::ezbake::security::thrift::EzSecurityPrincipal& principal,
        const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        verifies = securityConfig.verify(serializePrincipal(principal), principal.validity.signature);
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

::std::string EzSecurityTokenUtils::serializeTokenRequest(const ::ezbake::security::thrift::TokenRequest& token) {
    ::std::ostringstream ss;
    if (token.__isset.proxyPrincipal) {
        ss << token.proxyPrincipal.proxyToken << token.proxyPrincipal.signature;
    } else if (token.__isset.tokenPrincipal) {
        ss << serializeToken(token.tokenPrincipal);
    }
    ss << serializeTokenType(token.type) << token.securityId << token.targetSecurityId
       << token.timestamp;
    return ss.str();
}

::std::string EzSecurityTokenUtils::serializeProxyUserToken(const ::ezbake::base::thrift::ProxyUserToken &token) {
    ::std::ostringstream ss;
    ss << token.x509.subject << token.x509.issuer << token.issuedBy << token.issuedTo;
    if (token.__isset.issuedFor) {
        ss << token.issuedFor;
    }
    ss << token.notAfter;
    if (token.notBefore > 0) {
        ss << token.notBefore;
    }
    return ss.str();
}

::std::string EzSecurityTokenUtils::serializeToken(const ::ezbake::security::thrift::EzSecurityToken& token) {
    ::std::ostringstream ss;

    ss << token.validity.issuedTo;
    if (!token.validity.issuedFor.empty()) {
        ss << token.validity.issuedFor;
    }
    ss << token.validity.notAfter << token.validity.notBefore << token.validity.issuedTime
       << serializeTokenType(token.type);

    ss << token.tokenPrincipal.principal;
    if (token.tokenPrincipal.__isset.requestChain) {
        BOOST_FOREACH(const ::std::string& chain, token.tokenPrincipal.requestChain) {
            ss << chain;
        }
    }

    if (!token.authorizationLevel.empty()) {
        ss << token.authorizationLevel;
    }
    if (token.__isset.authorizations) {
        if (token.authorizations.__isset.formalAuthorizations) {
            BOOST_FOREACH(const ::std::string& auth, token.authorizations.formalAuthorizations) {
                ss << auth;
            }
        }
    }

    if (token.__isset.externalCommunities) {
        ::std::pair< ::std::string, ::ezbake::base::thrift::CommunityMembership> entry;
        BOOST_FOREACH(entry, token.externalCommunities) {
            ::ezbake::base::thrift::CommunityMembership& community = entry.second;
            ss << community.name << community.type << community.organization;
            ::std::sort(community.groups.begin(), community.groups.end());
            BOOST_FOREACH(const ::std::string& group, community.groups) {
                ss << group;
            }
            ::std::sort(community.topics.begin(), community.topics.end());
            BOOST_FOREACH(const ::std::string& topic, community.topics) {
                ss << topic;
            }
            ::std::sort(community.regions.begin(), community.regions.end());
            BOOST_FOREACH(const ::std::string& region, community.regions) {
                ss << region;
            }
        }
    }

    ss << serializeBoolean(token.validForExternalRequest) << token.citizenship << token.organization;

    return ss.str();
}

::std::string EzSecurityTokenUtils::principalSignature(const ::ezbake::security::thrift::EzSecurityPrincipal& principal,
        const SecurityConfiguration& securityConfig) {
    try {
        return securityConfig.sign(serializePrincipal(principal));
    } catch (const ::std::exception& ex) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Unable to generate EzSecurityToken signature: " +
                ::boost::diagnostic_information(ex)));
    }
    return "";
}

::std::string EzSecurityTokenUtils::serializePrincipal(const ::ezbake::security::thrift::EzSecurityPrincipal& principal) {
    ::std::ostringstream ss;
    ss << principal.principal << principal.validity.notAfter;
    if (principal.__isset.requestChain) {
        BOOST_FOREACH(const ::std::string& request, principal.requestChain) {
            ss << request;
        }
    }
    return ss.str();
}

bool EzSecurityTokenUtils::verifyReceivedToken(const ::ezbake::security::thrift::EzSecurityToken& token, const ::std::string id,
        const SecurityConfiguration& securityConfig) {
    bool verifies = false;
    try {
        if (!token.validity.__isset.issuedFor) {
            BOOST_THROW_EXCEPTION(::std::runtime_error("In this case, optional field 'issuedFor' is considered required"));
        }

        // check the recipient app in the token
        if (token.validity.issuedFor == id) {
            //Check that token not expired
            if (token.validity.notAfter > CommonUtils::currentTimeMillis()) {
                //Check that token is valid
                if (verifyTokenSignature(token, securityConfig)) {
                    //Again, check the recipient app
                    if (token.validity.issuedFor == id) {
                        //Again, check expired
                        if (token.validity.notAfter > CommonUtils::currentTimeMillis()) {
                            verifies = true;
                        } else {
                            LOG4CXX_DEBUG(LOG, "Error validating received token - expired");
                        }
                    } else {
                        LOG4CXX_DEBUG(LOG, "Error validating received token - not for this app");
                    }
                } else {
                    LOG4CXX_DEBUG(LOG, "Error validating signature of received token");
                }
            } else {
                LOG4CXX_DEBUG(LOG, "Error validating received token - expired");
            }
        } else {
            LOG4CXX_DEBUG(LOG, "Error validating received token - not for this app");
        }
    } catch (const ::std::exception& ex) {
        LOG4CXX_ERROR(LOG, "Exception verifying TokenRequest signature: " << ::boost::diagnostic_information(ex));
    }
    return verifies;
}

int64_t EzSecurityTokenUtils::getEzSecurityTokenJsonExpiration(const EzSecurityTokenJson& token) {
    ::picojson::value token_json;
    ::std::string parse_error = ::picojson::parse(token_json, token.json);
    if (!parse_error.empty()) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Exception in deserializing token: " + parse_error));
    }

    if (!token_json.is<picojson::object>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument("JSON is not an object"));
    }

    auto token_obj = token_json.get<picojson::object>();

    auto expires = token_obj["expires"];
    if (!expires.is<int64_t>()) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument(
                    "JSON does not have required field \"expires\" or the field is not a number"));
    }

    return expires.get<int64_t>();
}

}}} // namespace ezbake::security::core
