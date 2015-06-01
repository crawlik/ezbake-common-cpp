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

#include "ezbake/configuration/helpers/ThriftConfiguration.h"

#include <stdexcept>

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

ThriftConfiguration::ThriftServerMode
ThriftConfiguration::ThriftServerMode::valueOf(const ::std::string& val) {
    ThriftConfiguration::ThriftServerMode mode = ThriftConfiguration::Simple;
    if (ThriftConfiguration::HsHa.toString() == val) {
        mode = ThriftConfiguration::HsHa;
    } else if (ThriftConfiguration::ThreadedPool.toString() == val) {
        mode = ThriftConfiguration::ThreadedPool;
    } else if (ThriftConfiguration::Simple.toString() != val){
        BOOST_THROW_EXCEPTION(std::invalid_argument(val + " is not a supported thrift server mode"));
    }

    return mode;
}

ThriftConfiguration::ThriftServerMode::ThriftServerMode(const ::std::string& name, bool isBlocking)
    : _name(name), _isBlocking(isBlocking) {
}

ThriftConfiguration::ThriftServerMode::~ThriftServerMode() {
}

bool ThriftConfiguration::ThriftServerMode::isBlocking() const {
    return _isBlocking;
}

const ::std::string& ThriftConfiguration::ThriftServerMode::toString() const {
    return _name;
}

bool ThriftConfiguration::ThriftServerMode::operator==(
        const ThriftConfiguration::ThriftServerMode& rhs) const {
    return ((_isBlocking == rhs._isBlocking) && (_name == rhs._name));
}

bool ThriftConfiguration::ThriftServerMode::operator!=(
        const ThriftConfiguration::ThriftServerMode& rhs) const {
    return !(*this == rhs);
}

const ThriftConfiguration::ThriftServerMode ThriftConfiguration::Simple =
    ThriftConfiguration::ThriftServerMode("Simple", true);

const ThriftConfiguration::ThriftServerMode ThriftConfiguration::HsHa =
    ThriftConfiguration::ThriftServerMode("HsHa", false);

const ThriftConfiguration::ThriftServerMode ThriftConfiguration::ThreadedPool =
    ThriftConfiguration::ThriftServerMode("ThreadedPool", true);

boost::shared_ptr<ThriftConfiguration> ThriftConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    boost::shared_ptr<ThriftConfiguration> p(
        new ThriftConfiguration(configuration, namespacePrefix));

    return p;
}

ThriftConfiguration::ThriftConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

ThriftConfiguration::~ThriftConfiguration() {
}

bool ThriftConfiguration::useSSL() const {
    return ezConfiguration.getBoolean(
        g_EzBakeProperty_constants.THRIFT_USE_SSL,
        true);
}

ThriftConfiguration::ThriftServerMode ThriftConfiguration::getServerMode() const {
    ::std::string mode = ezConfiguration.getString(
        g_EzBakeProperty_constants.THRIFT_SERVER_MODE,
        "Simple");

    return ThriftServerMode::valueOf(mode);
}

int ThriftConfiguration::getMaxIdleClients() const {
    return ezConfiguration.getInt(
        g_EzBakeProperty_constants.THRIFT_MAX_IDLE_CLIENTS,
        10);
}

long ThriftConfiguration::getMillisBetweenClientEvictionChecks() const {
    return ezConfiguration.getLong(
        g_EzBakeProperty_constants.THRIFT_MILLIS_BETWEEN_CLIENT_EVICTION_CHECKS,
        30 * 1000);
}

long ThriftConfiguration::getMillisIdleBeforeEviction() const {
    return ezConfiguration.getLong(
        g_EzBakeProperty_constants.THRIFT_MILLIS_IDLE_BEFORE_EVICTION,
        2 * 60 * 1000);
}

int ThriftConfiguration::getMaxPoolClients() const {
    return ezConfiguration.getInt(
        g_EzBakeProperty_constants.THRIFT_MAX_POOL_CLIENTS,
        10);
}

bool ThriftConfiguration::getTestOnBorrow() const {
    return ezConfiguration.getBoolean(
        g_EzBakeProperty_constants.THRIFT_TEST_ON_BORROW,
        false);
}

bool ThriftConfiguration::getTestWhileIdle() const {
    return ezConfiguration.getBoolean(
        g_EzBakeProperty_constants.THRIFT_TEST_WHILE_IDLE,
        false);
}

}}} // namespace ezbake::configuration::helpers
