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

#include "ezbake/configuration/helpers/PostgresConfiguration.h"

#include "EzBakeProperty_constants.h"

extern "C" char **environ;

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<PostgresConfiguration> PostgresConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<PostgresConfiguration> p(
        new PostgresConfiguration(configuration, namespacePrefix));

    return p;
}

PostgresConfiguration::PostgresConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
    for (char **itr = environ; *itr; itr++) {
        ::std::string env(*itr);
        size_t delim = env.find("=");
        if (delim != ::std::string::npos) {
            _environment[env.substr(0, delim)] = env.substr(delim + 1);
        }
    }
}

PostgresConfiguration::~PostgresConfiguration() {
}

::std::string PostgresConfiguration::lookup(
        const ::std::string& ezKey, const ::std::string& envVar) const {
    ::boost::optional< ::std::string> value = get(ezKey);
    if (value) {
        return value.get();
    }

    auto map_iter = _environment.find(envVar);
    if (map_iter == _environment.end()) {
        return "";
    }

    return map_iter->second;
}

::std::string PostgresConfiguration::getHost() const {
    return lookup(
        g_EzBakeProperty_constants.POSTGRES_HOST,
        "OPENSHIFT_POSTGRESQL_DB_HOST");
}

::std::string PostgresConfiguration::getPort() const {
    return lookup(
        g_EzBakeProperty_constants.POSTGRES_PORT,
        "OPENSHIFT_POSTGRESQL_DB_PORT");
}

::std::string PostgresConfiguration::getUsername() const {
    return lookup(
        g_EzBakeProperty_constants.POSTGRES_USERNAME,
        "OPENSHIFT_POSTGRESQL_DB_USERNAME");
}

::std::string PostgresConfiguration::getPassword() const {
    return lookup(
        g_EzBakeProperty_constants.POSTGRES_PASSWORD,
        "OPENSHIFT_POSTGRESQL_DB_PASSWORD");
}

::std::string PostgresConfiguration::getDatabase() const {
    return lookup(
        g_EzBakeProperty_constants.POSTGRES_DB,
        "PGDATABASE");
}

::std::string PostgresConfiguration::getConnectionUrl() const {
    return "postgresql://" + getHost() + ":" + getPort();
}

void PostgresConfiguration::setEnvironment(
        const ::std::map< ::std::string, ::std::string>& environment) {
    _environment = environment;
}

}}} // namespace ::ezbake::configuration::configuration
