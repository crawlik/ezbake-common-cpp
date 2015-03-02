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

#ifndef EZBAKE_SECURITY_CLIENT_COMMONCLIENT_H_
#define EZBAKE_SECURITY_CLIENT_COMMONCLIENT_H_

#include "EzSecurity.h"
#include "ezbake/thriftutils/ThriftClientPoolBase.h"

namespace ezbake { namespace security { namespace client {

class CommonClient {
public:
    enum VerifySecurityTokenResponse {
        PASSED,
        FAILED,
        EXPIRED
    };

public:
    //amount of time to cache a connection since last use
    static const uint64_t CLIENT_CACHE_LIFETIME = 180; //seconds
    static const int64_t PRINCIPAL_EXPIRY = 1 * 1000; //millis

public:
    /**
     * Constructor
     */
    CommonClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                 const ::std::string& configNamespace="");
    CommonClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                 const ::std::string& configNamespace, const ::std::string& appName);

    /**
     * Destructor
     */
    virtual ~CommonClient() {}

    /**
     * Release thrift pool resources
     */
    virtual void close() = 0;

    /**
     * Return reference to the security configuration used by this client
     */
    virtual const ::ezbake::configuration::helpers::SecurityConfiguration& getSecurityConfig() const = 0;

protected:
    /**
     * Fetch the proxy token response from local cache
     *
     * @param request proxy token
     * @return Proxy token response
     * @throws EzSecurityTokenException
     * @throws UserNotFoundException
     * @throws TException
     */
    ::boost::optional< ::ezbake::security::thrift::ProxyTokenResponse> fetchProxyTokenFromCache(const ::ezbake::security::thrift::ProxyTokenRequest& request);

    /**
     * Fetch the security token json from local cache
     *
     * @param request token we're looking for
     * @return Security JSON object returned by security service
     * @throws EzSecurityTokenException
     * @throws TException
     */
    ::boost::optional< ::ezbake::base::thrift::EzSecurityTokenJson> fetchUserJsonFromCache(const ::ezbake::base::thrift::TokenRequest& request);

    /**
     * Generates a cache key for the specified request
     */
    inline static ::std::string getCacheKey(const ::ezbake::base::thrift::TokenRequest& request) {
        return request.principal.principal + request.securityId;
    }

    inline static ::std::string getCacheKey(const ::ezbake::security::thrift::ProxyTokenRequest& request) {
        ::std::string key = request.x509.subject;
        if (request.x509.__isset.issuer) {
            key += request.x509.issuer;
        }
        return key;
    }

protected: //data members
    ::std::string _appName; //security service app name
    ::boost::shared_ptr< ::ezbake::common::lrucache::LRUTimedCache< ::std::string,
                                                                    ::ezbake::security::thrift::ProxyTokenResponse> > _proxyTokenCache;
    ::boost::shared_ptr< ::ezbake::common::lrucache::LRUTimedCache< ::std::string,
                                                                    ::ezbake::base::thrift::EzSecurityTokenJson> > _userJsonCache;
};

}}} //::ezbake::security::client

#endif /* EZBAKE_SECURITY_CLIENT_COMMONCLIENT_H_ */
