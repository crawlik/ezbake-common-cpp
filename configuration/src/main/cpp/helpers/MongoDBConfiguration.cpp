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

#include "ezbake/configuration/helpers/MongoDBConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<MongoDBConfiguration> MongoDBConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<MongoDBConfiguration> p(
        new MongoDBConfiguration(configuration, namespacePrefix));

    return p;
}

MongoDBConfiguration::MongoDBConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

MongoDBConfiguration::~MongoDBConfiguration() {
}

::std::string MongoDBConfiguration::getMongoDBHostName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.MONGODB_HOST_NAME),
        "");
}

::std::string MongoDBConfiguration::getMongoDBDatabaseName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.MONGODB_DB_NAME),
        "");
}

::std::string MongoDBConfiguration::getMongoDBUserName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.MONGODB_USER_NAME),
        "");
}

::std::string MongoDBConfiguration::getMongoDBPassword() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.MONGODB_PASSWORD),
        "");
}

bool MongoDBConfiguration::useMongoDBSSL() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.MONGODB_USE_SSL),
        false);
}

}}} // namespace ::ezbake::configuration::helpers
