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

#include "ezbake/security/client/CommonClient.h"
#include "ezbake/security/core/CommonUtils.h"
#include "ezbake/security/core/EzSecurityTokenUtils.h"

namespace {
//initialize logger
log4cxx::LoggerPtr const LOG = log4cxx::Logger::getLogger("ezbake.security.client.CommonClient");
}

namespace ezbake { namespace security { namespace client {

using namespace ::ezbake::security::core;

using ::ezbake::thriftutils::ThriftClientPoolBase;
using ::ezbake::security::thrift::ProxyTokenResponse;
using ::ezbake::security::thrift::ProxyTokenRequest;
using ::ezbake::base::thrift::EzSecurityTokenJson;
using ::ezbake::base::thrift::TokenRequest;
using ::ezbake::base::thrift::ProxyUserToken;

using ::ezbake::configuration::EZConfiguration;
using ::ezbake::configuration::helpers::SecurityConfiguration;
using ::ezbake::common::lrucache::LRUTimedCache;


CommonClient::CommonClient(const EZConfiguration& ezconfig, const ::std::string& configNamespace) {
    ::boost::shared_ptr<SecurityConfiguration> config = SecurityConfiguration::fromConfiguration(ezconfig, configNamespace);
    _proxyTokenCache = ::boost::make_shared<LRUTimedCache<std::string, ProxyTokenResponse> >(config->getUserCacheSize(),
                                                                                             config->getUserCacheTTL());
    _userJsonCache = ::boost::make_shared<LRUTimedCache<std::string, EzSecurityTokenJson> >(config->getUserCacheSize(),
                                                                                            config->getUserCacheTTL());
}

CommonClient::CommonClient(const EZConfiguration& ezconfig, const ::std::string& configNamespace,
        const ::std::string& appName) : _appName(appName) {
    ::boost::shared_ptr<SecurityConfiguration> config = SecurityConfiguration::fromConfiguration(ezconfig, configNamespace);
    _proxyTokenCache = ::boost::make_shared<LRUTimedCache<std::string, ProxyTokenResponse> >(config->getUserCacheSize(),
                                                                                             config->getUserCacheTTL());
    _userJsonCache = ::boost::make_shared<LRUTimedCache<std::string, EzSecurityTokenJson> >(config->getUserCacheSize(),
                                                                                            config->getUserCacheTTL());
}

::boost::optional<ProxyTokenResponse> CommonClient::fetchProxyTokenFromCache(const ProxyTokenRequest& request) {
    ::std::string cacheKey = getCacheKey(request);
    LOG4CXX_DEBUG(LOG, "Attempting to get ProxyTokenResponse from cache. Using cache key " << cacheKey);

    ::boost::optional<ProxyTokenResponse> token_opt = _proxyTokenCache->get(cacheKey);
    if (token_opt) { //available in cache
        LOG4CXX_DEBUG(LOG, "ProxyTokenResponse available in cache");

        if (EzSecurityTokenUtils::isProxyTokenResponseExpired(*token_opt)) {
            LOG4CXX_DEBUG(LOG, "ProxyTokenReponse from cache has expired. Purging");
            //has expired, remove from cache
            _proxyTokenCache->remove(cacheKey);
        } else {
            return token_opt;
        }
    }
    LOG4CXX_DEBUG(LOG, "ProxyTokenResponse NOT available in cache");
    return ::boost::optional<ProxyTokenResponse>();
}

::boost::optional<EzSecurityTokenJson> CommonClient::fetchUserJsonFromCache(const TokenRequest& request) {
    ::std::string cacheKey = getCacheKey(request);

    LOG4CXX_DEBUG(LOG, "Attempting to get EzSecurityTokenJson from cache. Using cache key " << cacheKey);

    ::boost::optional<EzSecurityTokenJson> json_opt = _userJsonCache->get(cacheKey);
    if (json_opt) {//available in cache
        LOG4CXX_DEBUG(LOG, "EzSecurityTokenJson available in cache");

        if (EzSecurityTokenUtils::isEzSecurityTokenJsonExpired(*json_opt)) {
            LOG4CXX_DEBUG(LOG, "EzSecurityTokenJson from cache has expired. Purging");
            //has expired, remove from cache
            _userJsonCache->remove(cacheKey);
        } else {
            return json_opt;
        }
    }
    LOG4CXX_DEBUG(LOG, "EzSecurityTokenJson NOT available in cache");
    return ::boost::optional<EzSecurityTokenJson>();
}

}}} // namespace ezbake::security::client
