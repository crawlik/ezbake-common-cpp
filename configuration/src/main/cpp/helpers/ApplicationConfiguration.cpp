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

#include "ezbake/configuration/helpers/ApplicationConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

const char ApplicationConfiguration::APPLICATION_SERVICE_SEPARATOR = '_';

::std::string ApplicationConfiguration::getApplicationServiceName(
        const ::std::string& applicationName, const ::std::string& serviceName) {
    return applicationName + APPLICATION_SERVICE_SEPARATOR + serviceName;
}

::boost::shared_ptr<ApplicationConfiguration> ApplicationConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<ApplicationConfiguration> p(
        new ApplicationConfiguration(configuration, namespacePrefix));

    return p;
}

ApplicationConfiguration::ApplicationConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

ApplicationConfiguration::~ApplicationConfiguration() {
}

::std::string ApplicationConfiguration::getApplicationName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_APPLICATION_NAME),
        "");
}

::std::string ApplicationConfiguration::getApplicationVersion() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_APPLICATION_VERSION),
        "");
}

::std::string ApplicationConfiguration::getServiceName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_SERVICE_NAME),
        "");
}

::std::string ApplicationConfiguration::getSecurityID() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_SECURITY_ID),
        "");
}

::std::string ApplicationConfiguration::getCertificatesDir() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.EZBAKE_CERTIFICATES_DIRECTORY),
        "");
}

}}} // namespace ::ezbake::configuration::helpers
