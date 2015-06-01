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

#include "ezbake/configuration/helpers/ZookeeperConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<ZookeeperConfiguration> ZookeeperConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<ZookeeperConfiguration> p(
        new ZookeeperConfiguration(configuration, namespacePrefix));

    return p;
}

ZookeeperConfiguration::ZookeeperConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

ZookeeperConfiguration::~ZookeeperConfiguration() {
}

::std::string ZookeeperConfiguration::getZookeeperConnectionString() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ZOOKEEPER_CONNECTION_STRING),
        "");
}

}}} // namespace ::ezbake::configuration::helpers
