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

#ifndef EZBAKE_SECURITY_CLIENT_ASYNCCLIENT_H_
#define EZBAKE_SECURITY_CLIENT_ASYNCCLIENT_H_

#include "ezbake/security/client/CommonClient.h"
#include "ezbake/thriftutils/ThriftAsyncClientPool.h"

namespace ezbake { namespace security { namespace client {

class AsyncClient : public CommonClient {
public:
    typedef ::boost::function<void(const boost::shared_ptr< ::std::exception >&, bool )> PingCallback;
    typedef ::boost::function<void(const boost::shared_ptr< ::std::exception >&,
                                   const boost::shared_ptr< ::ezbake::security::thrift::ProxyTokenResponse>& )> ProxyTokenCallback;
    typedef ::boost::function<void(const boost::shared_ptr< ::std::exception >&,
                                   const boost::shared_ptr< ::ezbake::base::thrift::EzSecurityTokenJson>& )> UserJsonCallback;

public:
    /**
     * Constructor/Destructor
     */
    AsyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                const ::std::string& configNamespace="");
    AsyncClient(const ::ezbake::configuration::EZConfiguration& ezconfig,
                const ::std::string& configNamespace, const ::std::string& appName);
    virtual ~AsyncClient();

    /**
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
     * @param   cb  callback to dispatch to asynchronously handle response
     */
    void ping(const PingCallback& cb);

    /**
     * Request user DN from the ezbake security service.
     * If ProxyTokenResponse is available in cache, this function returns it without dispatching an async
     * call to the security service - the callback (cb) never gets called.
     * If not, it returns an empty optional and dispatches an async call to the security service; invoking the callback (cb)
     * on completion
     *
     * @param   cb          callback to dispatch to asynchronously handle response
     * @param   request     token we're looking for
     * @return  ProxyTokenReponse if available from cache, else an empty optional
     */
    ::boost::optional< ::ezbake::security::thrift::ProxyTokenResponse> fetchProxyToken(const ProxyTokenCallback& cb, const ::ezbake::security::thrift::ProxyTokenRequest& request);

    /**
     * Request user JSON from the ezbake security service
     * If EzSecurityTokenJson is available in cache, this function returns it without dispatching an async
     * call to the security service - the callback (cb) never gets called.
     * If not, it returns an empty optional and dispatches an async call to the security service; invoking the callback (cb)
     * on completion
     *
     * @param   cb          callback to dispatch to asynchronously handle response
     * @param   request     token we're looking for
     * @return  EzSecurityTokenJson if available from cache, else an empty optional
     */
    ::boost::optional< ::ezbake::base::thrift::EzSecurityTokenJson> fetchUserJson(const UserJsonCallback& cb, const ::ezbake::base::thrift::TokenRequest& request);

protected:
    void dispatchPing(const ::boost::shared_ptr< ::apache::thrift::TException>& err,
            const ::boost::shared_ptr< ::ezbake::base::thrift::EzBakeBaseServiceCobClient>& clientReturned,
            const PingCallback& cb);

    void dispatchFetchProxyToken(const ::boost::shared_ptr< ::apache::thrift::TException>& err,
            const ::boost::shared_ptr< ::ezbake::base::thrift::EzBakeBaseServiceCobClient>& clientReturned,
            const ProxyTokenCallback& cb, const ::ezbake::security::thrift::ProxyTokenRequest& request);


    void dispatchFetchUserJson(const ::boost::shared_ptr< ::apache::thrift::TException>& err,
            const ::boost::shared_ptr< ::ezbake::base::thrift::EzBakeBaseServiceCobClient>& clientReturned,
            const UserJsonCallback& cb, const ::ezbake::base::thrift::TokenRequest& request);

    /*
     * Even though thrift service API provides a raw pointer client (client that dispatched the async service)
     * to the callback handler, we still need to pass the client (share_ptr) so as not to prematurely destroy the
     * client object before the handler returns
     */
    void handlePingResponse(::ezbake::base::thrift::EzBakeBaseServiceCobClient* c_rtn,
            const PingCallback& cb, const ::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityCobClient>& client);

    void handleFetchProxyTokenResponse(::ezbake::security::thrift::EzSecurityCobClient* c_rtn, const ProxyTokenCallback& cb,
            const ::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityCobClient>& client,
            const ::ezbake::security::thrift::ProxyTokenRequest& request);

    void handleFetchUserJsonResponse(::ezbake::security::thrift::EzSecurityCobClient* c_rtn, const UserJsonCallback& cb,
            const ::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityCobClient>& client,
            const ::ezbake::base::thrift::TokenRequest& request);

private:
    ::ezbake::thriftutils::ThriftAsyncClientPool _pool;
};

}}} // ::ezbake::security::client

#endif /* EZBAKE_SECURITY_CLIENT_ASYNCCLIENT_H_ */
