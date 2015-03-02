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

#include "ezbake/configuration/util/HostAndPort.h"

#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

namespace ezbake { namespace configuration { namespace util {

const char HostAndPort::DELIM = ':';
const unsigned int HostAndPort::MIN_PORT = 0;
const unsigned int HostAndPort::MAX_PORT = 65535;

HostAndPort HostAndPort::fromHost(const ::std::string& host) {
    return HostAndPort(host);
}

HostAndPort HostAndPort::fromParts(const ::std::string& host, unsigned int port) {
    return HostAndPort(host, port);
}

HostAndPort HostAndPort::fromString(const ::std::string& str) {
    size_t delimPosition = str.find(DELIM);

    if (::std::string::npos == delimPosition) {
        return fromHost(str);
    }

    unsigned int port = ::boost::lexical_cast<unsigned int>(str.substr(delimPosition+1));
    return fromParts(str.substr(0, delimPosition), port);
}

HostAndPort::HostAndPort(const ::std::string& host, unsigned int port) {
    if (port > MAX_PORT) {
        BOOST_THROW_EXCEPTION(::std::out_of_range("port out of range"));
    }

    _host = host;
    _port = port;
}

HostAndPort::HostAndPort(const ::std::string& host) {
    _host = host;
}

::std::string HostAndPort::getHostText() const {
    return _host;
}

unsigned int HostAndPort::getPort() const {
    if (!_port) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument("no port defined"));
    }
    return *_port;
}

unsigned int HostAndPort::getPortOrDefault(unsigned int defaultPort) const {
    try {
        return getPort();
    } catch (const ::std::invalid_argument& e) {
        return defaultPort;
    }
}

bool HostAndPort::hasPort() const {
    return (_port ? true : false);
}

::std::string HostAndPort::toString() const {
    return (hasPort() ? _host + DELIM + ::std::to_string(static_cast<long long unsigned int>(*_port))
                      : _host + DELIM);
}

bool HostAndPort::operator==(const HostAndPort& rhs) const {
    if (this->_host == rhs._host) {
        if (this->hasPort() && rhs.hasPort()) {
            return (this->getPort() == rhs.getPort());
        } else if (!this->hasPort() && !rhs.hasPort()) {
            return true;
        }
    }
    return false;
}

bool HostAndPort::operator!=(const HostAndPort& rhs) const {
    return !(*this == rhs);
}

bool HostAndPort::operator<(const HostAndPort& rhs) const {
    if (this->_host == rhs._host) {
        if (this->hasPort() && rhs.hasPort()) {
            return (this->getPort() < rhs.getPort());
        } else if (this->hasPort()) {
            return false;
        }
        return true;
    }

    return (this->_host < rhs._host);
}

}}} //namespace ::ezbake::configuration::util

::std::ostream& operator<<(
        ::std::ostream& stream,
        const ::ezbake::configuration::util::HostAndPort& hostAndPort) {
    stream << hostAndPort.toString();
    return stream;
}

