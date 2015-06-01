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

#include "ezbake/configuration/helpers/RedisConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

const int RedisConfiguration::DEFAULT_REDIS_PORT = 6379;

::boost::shared_ptr<RedisConfiguration> RedisConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<RedisConfiguration> p(
        new RedisConfiguration(configuration, namespacePrefix));

    return p;
}

RedisConfiguration::~RedisConfiguration() {
}

RedisConfiguration::RedisConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

::std::string RedisConfiguration::getRedisHost() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.REDIS_HOST),
        "");
}

int RedisConfiguration::getRedisPort() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.REDIS_PORT),
        DEFAULT_REDIS_PORT);
}

}}} // namespace ezbake::configuration::helpers
