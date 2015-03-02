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

#ifndef EZBAKE_EZCONFIGURATION_HELPERS_THRIFTCONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_HELPERS_THRIFTCONFIGURATION_H_

#include <boost/shared_ptr.hpp>

#include "ezbake/configuration/helpers/BaseConfiguration.h"

namespace ezbake { namespace configuration { namespace helpers {

class ThriftConfiguration: public BaseConfiguration {
public:
    class ThriftServerMode {
        friend class ThriftConfiguration;
    public:
        static ThriftServerMode valueOf(const ::std::string& val);

        virtual ~ThriftServerMode();

        bool isBlocking() const;

        const ::std::string& toString() const;
        bool operator==(const ThriftServerMode& rhs) const;
        bool operator!=(const ThriftServerMode& rhs) const;

    private:
        ThriftServerMode(const ::std::string& name, bool isBlocking);

        ::std::string _name;
        bool _isBlocking;
    };

    friend class ThriftServerMode;

public:
    static const ThriftServerMode Simple;
    static const ThriftServerMode HsHa;
    static const ThriftServerMode ThreadedPool;

public:
    static boost::shared_ptr<ThriftConfiguration> fromConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix = "");

    virtual ~ThriftConfiguration();

    bool useSSL() const;
    ThriftServerMode getServerMode() const;
    int getMaxIdleClients() const;
    long getMillisBetweenClientEvictionChecks() const;
    long getMillisIdleBeforeEviction() const;
    int getMaxPoolClients() const;
    bool getTestOnBorrow() const;
    bool getTestWhileIdle() const;

private:
    ThriftConfiguration(
            const ::ezbake::configuration::EZConfiguration& configuration,
            const ::std::string& namespacePrefix);
};

}}} // namespace ezbake::configuration::helpers

#endif /* EZBAKE_EZCONFIGURATION_HELPERS_THRIFTCONFIGURATION_H_ */
