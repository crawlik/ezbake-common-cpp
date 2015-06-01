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

#ifndef EZBAKE_EZCONFIGURATION_HELPERS_BASECONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_HELPERS_BASECONFIGURATION_H_

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include "ezbake/configuration/EZConfiguration.h"

namespace ezbake { namespace configuration { namespace helpers {

class BaseConfiguration : public boost::noncopyable {
public:
    BaseConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix = "");

    virtual ~BaseConfiguration();

    ::boost::optional< ::std::string> get(const ::std::string& key) const;

protected:
    ::std::string parseKey(const ::std::string& key) const;

protected:
    ::ezbake::configuration::EZConfiguration ezConfiguration;
    ::std::string namespacePrefix;
};

}}} // namespace ::ezbake::configuration::helpers

#endif /* EZBAKE_EZCONFIGURATION_HELPERS_BASECONFIGURATION_H_ */
