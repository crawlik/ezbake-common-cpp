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

#include <log4cxx/logger.h>

#include "EzSecurityServices_constants.h"
#include "EzSecurityServices_types.h"
#include "ezbake/security/client/SyncClient.h"
#include "ezbake/security/core/EzSecurityTokenUtils.h"

namespace {
//create pointer to our logger
log4cxx::LoggerPtr const LOG = log4cxx::Logger::getLogger("ezbake.security.client.SyncClient");
}

namespace ezbake { namespace security { namespace client {

using ::ezbake::security::core::EzSecurityTokenUtils;
using ::ezbake::base::thrift::EzSecurityToken;
using ::ezbake::base::thrift::EzSecurityPrincipal;
using ::ezbake::base::thrift::EzSecurityTokenJson;
using ::ezbake::base::thrift::TokenRequest;
using ::ezbake::base::thrift::TokenType;
using ::ezbake::base::thrift::EzSecurityTokenException;
using ::ezbake::configuration::helpers::SecurityConfiguration;
using ::ezbake::security::thrift::EzSecurityClient;
using ::ezbake::security::thrift::UserNotFoundException;
using ::ezbake::security::thrift::g_EzSecurityServices_constants;
using ::ezbake::security::thrift::ProxyTokenRequest;
using ::ezbake::security::thrift::ProxyTokenResponse;

SyncClient::SyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                       const std::string& configNamespace)
    : CommonClient(ezconfig, configNamespace),
      _pool(ezconfig, configNamespace, CLIENT_CACHE_LIFETIME) {
    _pool.getSecurityConfiguration()->setKeys();
}

SyncClient::SyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                       const ::std::string& configNamespace, const ::std::string& appName)
    : CommonClient(ezconfig, configNamespace, appName),
      _pool(ezconfig, configNamespace, CLIENT_CACHE_LIFETIME) {
    _pool.getSecurityConfiguration()->setKeys();
}

SyncClient::~SyncClient() {
    close();
}

void SyncClient::close() {
    _pool.close();
}

const SecurityConfiguration& SyncClient::getSecurityConfig() const {
    return *(_pool.getSecurityConfiguration());
}

bool SyncClient::ping() {
    ::boost::shared_ptr<EzSecurityClient> client = getClient();
    bool ping = client->ping();
    returnClient(client);
    return ping;
}

ProxyTokenResponse SyncClient::fetchProxyToken(const ProxyTokenRequest& request) {
    ::boost::optional<ProxyTokenResponse> tokenOption = fetchProxyTokenFromCache(request);
    if (tokenOption) {
        LOG4CXX_DEBUG(LOG,
                "using ProxyTokenResponse from cache for user: " << request.x509.subject);
        return *tokenOption;
    }

    ::boost::shared_ptr<EzSecurityClient> client = getClient();
    ProxyTokenResponse rtnToken;

    try {
        LOG4CXX_DEBUG(LOG,
                "ProxyTokenResponse not available in cache. Requesting from security service for user: " <<
                request.x509.subject);
        client->requestProxyToken(rtnToken, request);
        LOG4CXX_DEBUG(LOG, "Received EzSecurityPrincipal");
        _proxyTokenCache->put(getCacheKey(request), rtnToken);
    } catch (const EzSecurityTokenException &e) {
        EzSecurityTokenException ex;
        ex.message = "Received response from security service, but got exception in fetching token: " + e.message;
        BOOST_THROW_EXCEPTION(ex);
    } catch (const UserNotFoundException &e) {
        EzSecurityTokenException ex;
        ex.message = "Received response from security service, user was not found: " + e.message;
        BOOST_THROW_EXCEPTION(ex);
    }

    returnClient(client);
    return rtnToken;
}

EzSecurityTokenJson SyncClient::fetchUserJson(const TokenRequest& request) {
    ::boost::optional<EzSecurityTokenJson> jsonOption = fetchUserJsonFromCache(request);
    if (jsonOption) {
        LOG4CXX_DEBUG(LOG, "using EzSecurityTokenJson from cache for user: " +
                request.principal.principal);
        return *jsonOption;
    }

    ::boost::shared_ptr<EzSecurityClient> client = getClient();
    EzSecurityTokenJson rtnJson;

    try {
        LOG4CXX_DEBUG(LOG, "EzSecurityTokenJson not available in cache. Requesting from security service for user: " +
                request.principal.principal);
        client->requestUserInfoAsJson(rtnJson, request,
                EzSecurityTokenUtils::tokenRequestSignature(request, *(_pool.getSecurityConfiguration())));
        LOG4CXX_DEBUG(LOG, "Received EzSecurityTokenJson");
        _userJsonCache->put(getCacheKey(request), rtnJson);
    } catch (const EzSecurityTokenException &e) {
        EzSecurityTokenException ex;
        ex.message = "Received response from security service, but was unable to verify the data: " + e.message;
        BOOST_THROW_EXCEPTION(ex);
    }

    returnClient(client);
    return rtnJson;
}

::boost::shared_ptr<EzSecurityClient> SyncClient::getClient() {
    ::boost::shared_ptr<EzSecurityClient> client =
            _pool.getClient<EzSecurityClient>(g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);

    if (!client) {
        BOOST_THROW_EXCEPTION(std::runtime_error("NULL client returned form thrift client pool for service: " +
                g_EzSecurityServices_constants.SECURITY_SERVICE_NAME));
    }

    return client;
}

void SyncClient::returnClient(::boost::shared_ptr<EzSecurityClient>& client) {
    _pool.returnToPool<EzSecurityClient>(client);
}

}}} //::ezbake::security::client
