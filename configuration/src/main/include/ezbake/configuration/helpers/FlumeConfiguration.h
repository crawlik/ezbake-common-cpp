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

#ifndef EZBAKE_EZCONFIGURATION_HELPERS_FLUMECONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_HELPERS_FLUMECONFIGURATION_H_

#include <boost/shared_ptr.hpp>

#include "ezbake/configuration/helpers/BaseConfiguration.h"

namespace ezbake { namespace configuration { namespace helpers {

class FlumeConfiguration: public BaseConfiguration {
private:
    static const int DEFAULT_BATCH_SIZE;
    static const int DEFAULT_MAX_EVENTS;
    static const int DEFAULT_MAX_ATTEMPTS;
    static const int DEFAULT_CONNECT_ATTEMPTS;

    static const long long DEFAULT_RUN_INTERVAL;
    static const long long DEFAULT_SLEEP_INTERVAL;
    static const long long DEFAULT_CONNECT_TIMEOUT;
    static const long long DEFAULT_REQUEST_TIMEOUT;
    static const long long DEFAULT_MAX_BACKOFF;

    static const ::std::string DEFAULT_HEADERS;
    static const ::std::string DEFAULT_AGENT_TYPE;
    static const ::std::string DEFAULT_HOSTS;
    static const ::std::string DEFAULT_HOST_SELECTOR;

    static const bool DEFAULT_BACK_OFF;

public:
    static ::boost::shared_ptr<FlumeConfiguration> fromConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix = "");

    virtual ~FlumeConfiguration();

    int getBatchSize() const;
    int getMaxEvents() const;
    int getMaxAttempts() const;
    int getConnectAttempts() const;
    long long getRunInterval() const;
    long long getSleepInterval() const;
    long long getConnectTimeout() const;
    long long getRequestTimeout() const;
    long long getMaxBackoff() const;
    ::std::string getHeaders() const;
    ::std::string getAgentType() const;
    ::std::string getHosts() const;
    ::std::string getHostSelector() const;
    bool getBackoff() const;
    ::ezbake::configuration::util::PropertiesUtil::Properties getHostProperties() const;

    void setHostProperties(const ::std::string& sHosts);
    ::ezbake::configuration::util::PropertiesUtil::Properties getProperties();

private:
    FlumeConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix);

    static ::ezbake::configuration::util::PropertiesUtil::Properties _properties;
    static ::ezbake::configuration::util::PropertiesUtil::Properties _hostProperties;
};

}}} // namespace ::ezbake::configuration::helpers

#endif /* EZBAKE_EZCONFIGURATION_HELPERS_FLUMECONFIGURATION_H_ */
