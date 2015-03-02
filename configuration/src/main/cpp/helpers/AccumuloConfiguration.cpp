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

#include "ezbake/configuration/helpers/AccumuloConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<AccumuloConfiguration> AccumuloConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<AccumuloConfiguration> p(
        new AccumuloConfiguration(configuration, namespacePrefix));

    return p;
}

AccumuloConfiguration::AccumuloConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

AccumuloConfiguration::~AccumuloConfiguration() {
}

::std::string AccumuloConfiguration::getAccumuloInstance() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ACCUMULO_INSTANCE_NAME),
        "");
}

::std::string AccumuloConfiguration::getZookeepers() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ACCUMULO_ZOOKEEPERS),
        "");
}

bool AccumuloConfiguration::useMock() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.ACCUMULO_USE_MOCK),
        false);
}

::std::string AccumuloConfiguration::getUsername() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ACCUMULO_USERNAME),
        "");
}

::std::string AccumuloConfiguration::getPassword() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ACCUMULO_PASSWORD),
        "");
}

}}} // namespace ::ezbake::configuration::helpers
