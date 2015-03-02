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
#include "ezbake/security/client/AsyncClient.h"
#include "ezbake/security/core/EzSecurityTokenUtils.h"

namespace {
//create pointer to our logger
log4cxx::LoggerPtr const LOG = log4cxx::Logger::getLogger("ezbake.security.client.AsyncClient");
}

namespace ezbake { namespace security { namespace client {

using ::ezbake::security::core::EzSecurityTokenUtils;
using ::ezbake::base::thrift::EzSecurityPrincipal;
using ::ezbake::base::thrift::EzSecurityTokenJson;
using ::ezbake::base::thrift::TokenRequest;
using ::ezbake::base::thrift::EzBakeBaseServiceCobClient;
using ::ezbake::thriftutils::ThriftAsyncClientPool;
using ::ezbake::configuration::helpers::SecurityConfiguration;
using ::ezbake::security::thrift::EzSecurityCobClient;
using ::ezbake::security::thrift::g_EzSecurityServices_constants;
using ::ezbake::security::thrift::ProxyTokenResponse;
using ::ezbake::security::thrift::ProxyTokenRequest;
using ::ezbake::configuration::EZConfiguration;
using ::apache::thrift::TException;

AsyncClient::AsyncClient(const EZConfiguration& ezconfig,
                         const ::std::string& configNamespace)
    : CommonClient(ezconfig, configNamespace),
    _pool(ezconfig, configNamespace, CLIENT_CACHE_LIFETIME) {
    _pool.getSecurityConfiguration()->setKeys();
}

AsyncClient::AsyncClient(const EZConfiguration& ezconfig,
        const ::std::string& configNamespace, const ::std::string& appName)
    : CommonClient(ezconfig, configNamespace, appName),
      _pool(ezconfig, configNamespace, CLIENT_CACHE_LIFETIME) {
    _pool.getSecurityConfiguration()->setKeys();
}

AsyncClient::~AsyncClient() {
    try {
        close();
    } catch (...) {
        //do nothing
    }
}

void AsyncClient::close() {
    _proxyTokenCache->clear();
    _userJsonCache->clear();
    _pool.close();
}

const SecurityConfiguration& AsyncClient::getSecurityConfig() const {
    return *(_pool.getSecurityConfiguration());
}

void AsyncClient::ping(const PingCallback& cb) {
    ThriftAsyncClientPool::GetClientCallback callback =
            ::boost::bind(&AsyncClient::dispatchPing, this, _1, _2, cb);
    if (_appName.empty()) {
        _pool.getClient<EzSecurityCobClient>(callback, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
    } else {
        _pool.getClient<EzSecurityCobClient>(callback, _appName, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
    }
}

void AsyncClient::dispatchPing(const ::boost::shared_ptr<TException>& err,
                               const ::boost::shared_ptr<EzBakeBaseServiceCobClient>& clientReturned,
                               const PingCallback& cb) {
    bool result=false;
    ::boost::shared_ptr< ::std::runtime_error> rtnError;
    ::boost::shared_ptr<EzSecurityCobClient> client =
            ::boost::dynamic_pointer_cast<EzSecurityCobClient>(clientReturned);

    try {
        if (err) {
            BOOST_THROW_EXCEPTION(*err);
        }

        if (!client) {
            BOOST_THROW_EXCEPTION(::std::runtime_error("Error! Null client created"));
        }

        client->ping(::boost::bind(&AsyncClient::handlePingResponse, this, _1, cb, client));

        //return on successfully dispatching the ping
        return;
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in dispatching Ping: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    }

    //An error occurred. Invoke callback to report error
    cb(rtnError, result);
}

void AsyncClient::handlePingResponse(::ezbake::base::thrift::EzBakeBaseServiceCobClient* c_rtn,
        const PingCallback& cb, const ::boost::shared_ptr< ::ezbake::security::thrift::EzSecurityCobClient>& client) {
    bool result=false;
    ::boost::shared_ptr< ::std::runtime_error> rtnError;

    try {
        if (NULL == c_rtn) {
            BOOST_THROW_EXCEPTION(TException("Error! Null client returned in handling ping"));
        }

        //read the ping response
        result = client->recv_ping();
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in reading Ping async response: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    }

    //Done handling ping request. Invoke callback
    cb(rtnError, result);
}

::boost::optional<ProxyTokenResponse> AsyncClient::fetchProxyToken(const ProxyTokenCallback& cb,
        const ProxyTokenRequest& request) {
    ::boost::optional<ProxyTokenResponse> tokenOption = fetchProxyTokenFromCache(request);
    if (tokenOption) {
        LOG4CXX_DEBUG(LOG,
                "using ProxyTokenResponse from cache for user: " << request.x509.subject);
    } else {
        LOG4CXX_DEBUG(LOG,
                "dispatching new ProxyTokenRequest user: " << request.x509.subject);

        ThriftAsyncClientPool::GetClientCallback callback =
                ::boost::bind(&AsyncClient::dispatchFetchProxyToken, this, _1, _2, cb, request);
        if (_appName.empty()) {
            _pool.getClient<EzSecurityCobClient>(callback, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
        } else {
            _pool.getClient<EzSecurityCobClient>(callback, _appName, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
        }
    }

    return tokenOption;
}

void AsyncClient::dispatchFetchProxyToken(const ::boost::shared_ptr<TException>& err,
        const ::boost::shared_ptr<EzBakeBaseServiceCobClient>& clientReturned,
        const ProxyTokenCallback& cb, const ProxyTokenRequest& request) {
    ::boost::shared_ptr< ::std::runtime_error> rtnError;
    ::boost::shared_ptr<EzSecurityCobClient> client =
            ::boost::dynamic_pointer_cast<EzSecurityCobClient>(clientReturned);

    try {
        if (err) {
            BOOST_THROW_EXCEPTION(*err);
        }

        if (!client) {
            BOOST_THROW_EXCEPTION(::std::runtime_error("Error! Null client created"));
        }

        LOG4CXX_DEBUG(LOG, "Established connection to security service. Dispatching fetch for ProxyToken");

        client->requestProxyToken(::boost::bind(&AsyncClient::handleFetchProxyTokenResponse,
                                                this, _1, cb, client, request),
                                  request);
        return;
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in dispatching UserDn request: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    }

    //An error occurred. Invoke callback to report error
    cb(rtnError, ::boost::shared_ptr<ProxyTokenResponse>());
}

void AsyncClient::handleFetchProxyTokenResponse(EzSecurityCobClient* c_rtn, const ProxyTokenCallback& cb,
        const ::boost::shared_ptr<EzSecurityCobClient>& client, const ProxyTokenRequest& request) {
    ::boost::shared_ptr<ProxyTokenResponse> result;
    ::boost::shared_ptr< ::std::runtime_error> rtnError;

    try {
        if (NULL == c_rtn) {
            BOOST_THROW_EXCEPTION(TException("Error! Null client returned in handling UserDn response"));
        }

        LOG4CXX_DEBUG(LOG, "Successfully sent ProxyToken request to security service. Reading the response ...");

        //read the user DN response
        result = ::boost::make_shared<ProxyTokenResponse>();
        client->recv_requestProxyToken(*result);

        //verify data from security service
        if (!EzSecurityTokenUtils::verifyProxyUserToken(result->token,
                result->signature, *_pool.getSecurityConfiguration())) {
            LOG4CXX_ERROR(LOG, "Invalid signature for ProxyTokenResponse received from security service for"
                    << "user {" << request.x509.subject <<"}");
            ::ezbake::base::thrift::EzSecurityTokenException ex;
            ex.message = "Invalid signature for ProxyTokenResponse received form security service";
            BOOST_THROW_EXCEPTION(ex);
        }

        LOG4CXX_DEBUG(LOG, "Saving received ProxyTokenResponse to cache");
        _proxyTokenCache->put(getCacheKey(request), *result);

    } catch (const ::ezbake::base::thrift::EzSecurityTokenException &e) {
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e) +
                "[EzSecurityTokenException:" + e.message + ": " + e.originalException + "]");
    } catch (const ::ezbake::security::thrift::UserNotFoundException &e) {
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e) +
                "[UserNotFoundException:" + e.message + "]");
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in reading UserDn async response: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    } catch (...) {
        ::std::string errMessage = "Undetermined exception caught while handlingUserDnResponse: " +
                ::boost::current_exception_diagnostic_information();
        LOG4CXX_FATAL(LOG, errMessage);
        rtnError = ::boost::make_shared< ::std::runtime_error>(errMessage);
    }

    //Done handling fetch ProxyToken request. Invoke callback
    cb(rtnError, result);
}

::boost::optional<EzSecurityTokenJson> AsyncClient::fetchUserJson(const UserJsonCallback& cb,
        const ::ezbake::base::thrift::TokenRequest& request) {
    ::boost::optional<EzSecurityTokenJson> tokenOption = fetchUserJsonFromCache(request);
    if (tokenOption) {
        LOG4CXX_DEBUG(LOG, "using EzSecurityTokenJson from cache for user: " +
                request.principal.principal);
    } else {
        ThriftAsyncClientPool::GetClientCallback callback =
                ::boost::bind(&AsyncClient::dispatchFetchUserJson, this, _1, _2, cb, request);
        if (_appName.empty()) {
            _pool.getClient<EzSecurityCobClient>(callback, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
        } else {
            _pool.getClient<EzSecurityCobClient>(callback, _appName, g_EzSecurityServices_constants.SECURITY_SERVICE_NAME);
        }
    }
    return tokenOption;
}

void AsyncClient::dispatchFetchUserJson(const ::boost::shared_ptr< ::apache::thrift::TException>& err,
        const ::boost::shared_ptr< ::ezbake::base::thrift::EzBakeBaseServiceCobClient>& clientReturned,
        const UserJsonCallback& cb, const TokenRequest& request) {
    ::boost::shared_ptr< ::std::runtime_error> rtnError;
    ::boost::shared_ptr<EzSecurityCobClient> client =
            ::boost::dynamic_pointer_cast<EzSecurityCobClient>(clientReturned);

    try {
        if (err) {
            BOOST_THROW_EXCEPTION(*err);
        }

        if (!client) {
            BOOST_THROW_EXCEPTION(::std::runtime_error("Error! Null client created"));
        }

        client->requestUserInfoAsJson(::boost::bind(&AsyncClient::handleFetchUserJsonResponse,
                                                    this, _1, cb, client, request),
                                      request, EzSecurityTokenUtils::tokenRequestSignature(request,
                                                                                           *(_pool.getSecurityConfiguration())));
        return;
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in dispatching UserJson request: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    }

    //An error occurred. Invoke callback to report error
    cb(rtnError, ::boost::shared_ptr<EzSecurityTokenJson>());
}

void AsyncClient::handleFetchUserJsonResponse(EzSecurityCobClient* c_rtn, const UserJsonCallback& cb,
        const ::boost::shared_ptr<EzSecurityCobClient>& client, const ::ezbake::base::thrift::TokenRequest& request) {
    ::boost::shared_ptr<EzSecurityTokenJson> result;
    ::boost::shared_ptr< ::std::runtime_error> rtnError;

    try {
        if (NULL == c_rtn) {
            BOOST_THROW_EXCEPTION(TException("Error! Null client returned in handling UserJson response"));
        }

        //read the user DN response
        result = ::boost::make_shared<EzSecurityTokenJson>();
        client->recv_requestUserInfoAsJson(*result);
        _userJsonCache->put(getCacheKey(request), *result);

    } catch (const ::ezbake::base::thrift::EzSecurityTokenException &e) {
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e) +
                "[EzSecurityTokenException:" + e.message + ": " + e.originalException + "]");
    } catch (const ::std::exception& e) {
        LOG4CXX_ERROR(LOG, "Error in reading UserJson async response: " << e.what());
        rtnError = ::boost::make_shared< ::std::runtime_error>(::boost::diagnostic_information(e));
    } catch (...) {
        ::std::string errMessage = "Undetermined exception caught while handlingUserDnResponse: " +
                ::boost::current_exception_diagnostic_information();
        LOG4CXX_FATAL(LOG, errMessage);
        rtnError = ::boost::make_shared< ::std::runtime_error>(errMessage);
    }

    //Done handling fetch UserJson request. Invoke callback
    cb(rtnError, result);
}

}}} // ::ezbake::security::client

