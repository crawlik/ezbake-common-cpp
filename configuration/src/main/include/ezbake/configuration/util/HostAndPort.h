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

#ifndef EZBAKE_EZCONFIGURATION_UTIL_HOSTANDPORT_H_
#define EZBAKE_EZCONFIGURATION_UTIL_HOSTANDPORT_H_

#include <string>
#include <ostream>

#include <boost/optional.hpp>

namespace ezbake { namespace configuration { namespace util {

class HostAndPort
{
public:
    static const char DELIM;
    static const unsigned int MIN_PORT;
    static const unsigned int MAX_PORT;

    static HostAndPort fromHost(const ::std::string& host);
    static HostAndPort fromParts(const ::std::string& host, unsigned int port);
    static HostAndPort fromString(const ::std::string& str);

    ::std::string getHostText() const;
    unsigned int getPort() const;
    unsigned int getPortOrDefault(unsigned int defaultPort) const;
    bool hasPort() const;
    ::std::string toString() const;

    bool operator==(const HostAndPort& rhs) const;
    bool operator!=(const HostAndPort& rhs) const;
    bool operator<(const HostAndPort& rhs) const;

private:
    HostAndPort(const ::std::string& host, unsigned int port);
    HostAndPort(const ::std::string& host);

    ::std::string _host;
    ::boost::optional<unsigned int> _port;
};


}}} //namespace ::ezbake::configuration::util

::std::ostream& operator<<(
        ::std::ostream& stream,
        const ::ezbake::configuration::util::HostAndPort& hostAndPort);

#endif /* EZBAKE_EZCONFIGURATION_UTIL_HOSTANDPORT_H_ */
