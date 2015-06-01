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

#ifndef EZBAKE_SECURITY_CLIENT_SYNCCLIENT_H_
#define EZBAKE_SECURITY_CLIENT_SYNCCLIENT_H_

#include "ezbake/security/client/CommonClient.h"
#include "ezbake/thriftutils/ThriftSyncClientPool.h"

namespace ezbake { namespace security { namespace client {

class SyncClient : public CommonClient {
public:
    /**
     * Creates a new SyncClient for handling requests to security service
     */
    SyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
               const ::std::string& configNamespace="");
    SyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
               const ::std::string& configNamespace, const ::std::string& appName);

    /*
     * Destructor
     */
    virtual ~SyncClient();

    /*
     * Release thrift pool resources
     */
    virtual void close();

    /**
     * Return a reference to the underlying security configuration of this client
     */
    virtual const ::ezbake::configuration::helpers::SecurityConfiguration& getSecurityConfig() const;

    /**
     * Ping the security service
     *
     * @return True if able to ping security service. False otherwise
     * @throws TException
     */
    bool ping();

    /**
     * Request proxy token from the ezbake security service
     *
     * @param request proxy token
     * @return proxy token response
     * @throws EzSecurityTokenException
     * @throws UserNotFoundException
     * @throws TException
     */
    ::ezbake::security::thrift::ProxyTokenResponse fetchProxyToken(const ::ezbake::security::thrift::ProxyTokenRequest& request);

    /**
     * Request user JSON from the ezbake security service
     *
     * @param request token we're looking for
     * @return Security JSON object returned by security service
     * @throws EzSecurityTokenException
     * @throws TException
     */
    ::ezbake::base::thrift::EzSecurityTokenJson fetchUserJson(const ::ezbake::base::thrift::TokenRequest& request);

protected:
    /**
     * Get an EzSecurity client.
     * The client should be returned by calling returnClient()
     *
     * @return a reference to an EzSecurity Client
     * @throws TException
     */
    ::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityClient> getClient();

    /**
     * Returns an EzSecurity client.
     * The client returned should be the one obtained from a call to getClient()
     *
     * @param client to return
     */
    void returnClient(::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityClient>& client);

private:
    ::ezbake::thriftutils::ThriftSyncClientPool _pool;
};

}}} //::ezbake::security::client

#endif /* EZBAKE_SECURITY_CLIENT_SYNCCLIENT_H_ */
