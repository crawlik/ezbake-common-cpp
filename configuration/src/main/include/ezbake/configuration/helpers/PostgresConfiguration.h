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

#ifndef EZBAKE_EZCONFIGURATION_HELPERS_POSTGRESCONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_HELPERS_POSTGRESCONFIGURATION_H_

#include <map>

#include <boost/shared_ptr.hpp>

#include "ezbake/configuration/helpers/BaseConfiguration.h"

namespace ezbake { namespace configuration { namespace helpers {

class PostgresConfiguration: public BaseConfiguration {
public:
    static ::boost::shared_ptr<PostgresConfiguration> fromConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix = "");

    virtual ~PostgresConfiguration();

    ::std::string getHost() const;
    ::std::string getPort() const;
    ::std::string getUsername() const;
    ::std::string getPassword() const;
    ::std::string getDatabase() const;
    ::std::string getConnectionUrl() const;

protected:
    void setEnvironment(const ::std::map< ::std::string, ::std::string>& environment);

private:
    ::std::string lookup(const ::std::string& ezKey, const ::std::string& envVar) const;

    PostgresConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix);

private:
    ::std::map< ::std::string, ::std::string> _environment;
};

}}} // namespace ::ezbake::configuration::configuration

#endif /* EZBAKE_EZCONFIGURATION_HELPERS_POSTGRESCONFIGURATION_H_ */
