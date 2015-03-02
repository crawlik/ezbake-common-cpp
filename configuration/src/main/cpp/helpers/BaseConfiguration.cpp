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

#include "ezbake/configuration/helpers/BaseConfiguration.h"

#include <boost/format.hpp>

namespace ezbake { namespace configuration { namespace helpers {

BaseConfiguration::BaseConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : ezConfiguration(configuration), namespacePrefix(namespacePrefix) {
}

BaseConfiguration::~BaseConfiguration() {
}

::boost::optional< ::std::string> BaseConfiguration::get(const ::std::string& key) const {
    return ezConfiguration.get(parseKey(key));
}

::std::string BaseConfiguration::parseKey(const ::std::string& key) const {
    return (namespacePrefix.find_first_not_of(' ') == ::std::string::npos) ? key :
            (::boost::format("%s.%s") % namespacePrefix % key).str();
}

}}} // namespace ::ezbake::configuration::helpers
