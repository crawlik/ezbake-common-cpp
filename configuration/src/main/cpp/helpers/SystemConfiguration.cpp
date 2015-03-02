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

#include "ezbake/configuration/helpers/SystemConfiguration.h"

#include <stdexcept>

#include "EzBakeProperty_constants.h"
#include "ezbake/configuration/helpers/ApplicationConfiguration.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

const ::std::string SystemConfiguration::FILE_SEPARATOR = "/";

boost::shared_ptr<SystemConfiguration> SystemConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    boost::shared_ptr<SystemConfiguration> p(
        new SystemConfiguration(configuration, namespacePrefix));

    return p;
}

SystemConfiguration::SystemConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

SystemConfiguration::~SystemConfiguration() {
}

::std::string SystemConfiguration::getLogDirectory() const {
    return get(g_EzBakeProperty_constants.EZBAKE_LOG_DIRECTORY).get_value_or("/tmp");
}

::std::string SystemConfiguration::getLogFilePath(
        const ::std::string& applicationName, const ::std::string& serviceName) const {
    if (serviceName.find_first_not_of(' ') == ::std::string::npos) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("serviceName must not be blank!!"));
    }

    const ::std::string logext = ".log";
    ::std::string fileName, path = getLogDirectory();

    if (applicationName.find_first_not_of(' ') != ::std::string::npos) {
        path += FILE_SEPARATOR + applicationName;
        fileName = ApplicationConfiguration::getApplicationServiceName(applicationName, serviceName) + logext;
    } else {
        path += FILE_SEPARATOR + serviceName;
        fileName = serviceName + logext;
    }

    return path + FILE_SEPARATOR + fileName;
}

bool SystemConfiguration::shouldLogToStdOut() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.EZBAKE_LOG_TO_STDOUT),
        false);
}

bool SystemConfiguration::isAdminApplicationDeployment() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.EZBAKE_ADMIN_APPLICATION_DEPLOYMENT),
        false);
}

}}} // namespace ::ezbake::configuration::helpers
