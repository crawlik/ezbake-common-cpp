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

#include "ezbake/configuration/helpers/FlumeConfiguration.h"

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

const int FlumeConfiguration::DEFAULT_BATCH_SIZE = 100;
const int FlumeConfiguration::DEFAULT_MAX_EVENTS = 10000;
const int FlumeConfiguration::DEFAULT_MAX_ATTEMPTS = 2;
const int FlumeConfiguration::DEFAULT_CONNECT_ATTEMPTS = 10;

const long long FlumeConfiguration::DEFAULT_RUN_INTERVAL = 1000;
const long long FlumeConfiguration::DEFAULT_SLEEP_INTERVAL = 3000;
const long long FlumeConfiguration::DEFAULT_CONNECT_TIMEOUT = 20000;
const long long FlumeConfiguration::DEFAULT_REQUEST_TIMEOUT = 20000;
const long long FlumeConfiguration::DEFAULT_MAX_BACKOFF = 0;

const ::std::string FlumeConfiguration::DEFAULT_HEADERS = "";
const ::std::string FlumeConfiguration::DEFAULT_AGENT_TYPE = "SINGLE";
const ::std::string FlumeConfiguration::DEFAULT_HOSTS = "logAgent00 logAgent01";
const ::std::string FlumeConfiguration::DEFAULT_HOST_SELECTOR = "ROUND_ROBIN";

const bool FlumeConfiguration::DEFAULT_BACK_OFF = false;

::ezbake::configuration::util::PropertiesUtil::Properties FlumeConfiguration::_properties;
::ezbake::configuration::util::PropertiesUtil::Properties FlumeConfiguration::_hostProperties;

::boost::shared_ptr<FlumeConfiguration> FlumeConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<FlumeConfiguration> p(
        new FlumeConfiguration(configuration, namespacePrefix));

    return p;
}

FlumeConfiguration::FlumeConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

FlumeConfiguration::~FlumeConfiguration() {
}

int FlumeConfiguration::getBatchSize() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.FLUME_BATCH_SIZE),
        DEFAULT_BATCH_SIZE);
}

int FlumeConfiguration::getMaxEvents() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.FLUME_MAX_EVENTS),
        DEFAULT_MAX_EVENTS);
}

int FlumeConfiguration::getMaxAttempts() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.FLUME_MAX_ATTEMPTS),
        DEFAULT_MAX_ATTEMPTS);
}

int FlumeConfiguration::getConnectAttempts() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.FLUME_CONNECT_ATTEMPTS),
        DEFAULT_CONNECT_ATTEMPTS);
}

long long FlumeConfiguration::getRunInterval() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.FLUME_RUN_INTERVAL),
        DEFAULT_RUN_INTERVAL);
}

long long FlumeConfiguration::getSleepInterval() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.FLUME_SLEEP_INTERVAL),
        DEFAULT_SLEEP_INTERVAL);
}

long long FlumeConfiguration::getConnectTimeout() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.FLUME_CONNECT_TIMEOUT),
        DEFAULT_CONNECT_TIMEOUT);
}

long long FlumeConfiguration::getRequestTimeout() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.FLUME_REQUEST_TIMEOUT),
        DEFAULT_REQUEST_TIMEOUT);
}

long long FlumeConfiguration::getMaxBackoff() const {
    return ezConfiguration.getLong(
        parseKey(g_EzBakeProperty_constants.FLUME_MAX_BACKOFF),
        DEFAULT_MAX_BACKOFF);
}

::std::string FlumeConfiguration::getHeaders() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.FLUME_HEADERS),
        DEFAULT_HEADERS);
}

::std::string FlumeConfiguration::getAgentType() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.FLUME_AGENT_TYPE),
        DEFAULT_AGENT_TYPE);
}

::std::string FlumeConfiguration::getHosts() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.FLUME_HOSTS),
        DEFAULT_HOSTS);
}

::std::string FlumeConfiguration::getHostSelector() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.FLUME_HOST_SELECTOR),
        DEFAULT_HOST_SELECTOR);
}

bool FlumeConfiguration::getBackoff() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.FLUME_BACK_OFF),
        DEFAULT_BACK_OFF);
}

::ezbake::configuration::util::PropertiesUtil::Properties FlumeConfiguration::getHostProperties() const {
    return _hostProperties;
}

void FlumeConfiguration::setHostProperties(const ::std::string& sHosts) {
    ::boost::tokenizer< ::boost::char_separator<char> > hostTokens(sHosts, ::boost::char_separator<char>(" "));
    BOOST_FOREACH(const ::std::string& host, hostTokens) {
        ::std::string propName = "hosts." + host;
        _hostProperties[host] = ezConfiguration.getString(parseKey(propName), "");
    }
}

::ezbake::configuration::util::PropertiesUtil::Properties FlumeConfiguration::getProperties() {
    _properties[g_EzBakeProperty_constants.FLUME_BATCH_SIZE] = getBatchSize();
    _properties[g_EzBakeProperty_constants.FLUME_MAX_EVENTS] = getMaxEvents();
    _properties[g_EzBakeProperty_constants.FLUME_MAX_ATTEMPTS] = getMaxAttempts();
    _properties[g_EzBakeProperty_constants.FLUME_CONNECT_ATTEMPTS] = getConnectAttempts();

    _properties[g_EzBakeProperty_constants.FLUME_RUN_INTERVAL] = getRunInterval();
    _properties[g_EzBakeProperty_constants.FLUME_SLEEP_INTERVAL] = getSleepInterval();
    _properties[g_EzBakeProperty_constants.FLUME_CONNECT_TIMEOUT] = getConnectTimeout();
    _properties[g_EzBakeProperty_constants.FLUME_REQUEST_TIMEOUT] = getRequestTimeout();
    _properties[g_EzBakeProperty_constants.FLUME_MAX_BACKOFF] = getMaxBackoff();

    _properties[g_EzBakeProperty_constants.FLUME_HEADERS] = getHeaders();
    _properties[g_EzBakeProperty_constants.FLUME_AGENT_TYPE] = getAgentType();
    _properties[g_EzBakeProperty_constants.FLUME_HOSTS] = getHosts();
    _properties[g_EzBakeProperty_constants.FLUME_HOST_SELECTOR] = getHostSelector();

    _properties[g_EzBakeProperty_constants.FLUME_BACK_OFF] = getBackoff();

    setHostProperties(getHosts());
    return _properties;
}

}}} // namespace ::ezbake::configuration::helpers
