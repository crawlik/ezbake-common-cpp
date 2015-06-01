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


#include <cstdlib>

#include <boost/filesystem.hpp>
#include <log4cxx/logger.h>

#include "ezbake/configuration/EZConfiguration.h"
#include "ezbake/configuration/util/StringConversionUtil.h"
#include "ezbake/configuration/util/OpenShiftUtil.h"

namespace {
//create pointer to our logger
::log4cxx::LoggerPtr const LOG = ::log4cxx::Logger::getLogger("::ezbake::configuration::EZConfiguration");
}

namespace ezbake { namespace configuration {

using ::ezbake::configuration::util::PropertiesUtil;
using ::ezbake::configuration::util::StringConversionUtil;
using ::ezbake::configuration::util::OpenShiftUtil;

const ::std::string EZConfiguration::DEFAULT_EZCONFIGURATION_DIR = "/etc/sysconfig/ezbake";
const ::std::string EZConfiguration::EZCONFIGURATION_DIR_VAR = "EZCONFIGURATION_DIR";

EZConfiguration::EZConfiguration(bool loadDefaults) {
    if (loadDefaults) {
        try {
            LOG4CXX_INFO(LOG, "Loading EZConfiguration with Defaults");
            loadDefaultConfiguration();
        } catch (const ::std::exception& e) {
            BOOST_THROW_EXCEPTION(::std::runtime_error(::std::string("Could not load EZConfiguration defaults: ") + e.what()));
        }
    } else {
        LOG4CXX_DEBUG(LOG, "Loading empty EZConfiguration");
    }
}

EZConfiguration::EZConfiguration(const PropertiesUtil::Properties& props) {
    LOG4CXX_DEBUG(LOG, "Loading EZConfiguration with provided property container");
    _properties = props;
}

EZConfiguration::~EZConfiguration() {
}

::boost::optional< ::std::string> EZConfiguration::get(const ::std::string& key) const {
    boost::optional< ::std::string> retVal;

    if (::std::string::npos == key.find_first_not_of(' ')) {
        //blank key
        return retVal;
    }

    auto entry = _properties.find(key);
    if (entry == _properties.end()) {
        //property doesn't exist in the configuration
        return retVal;
    }

    retVal = entry->second;
    return retVal;
}

void EZConfiguration::set(const ::std::string& key, const ::std::string& value, bool shouldOverride) {
    if (::std::string::npos == key.find_first_not_of(' ')) {
        BOOST_THROW_EXCEPTION(::std::invalid_argument("invalid key"));
    }

    if (!shouldOverride && (_properties.find(key) != _properties.end())) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Key {" + key + "} has already been set!"));
    }

    _properties.insert(PropertiesUtil::Property(key, value));
}

bool EZConfiguration::getBoolean(const ::std::string& key, bool defaultValue) const {
    return StringConversionUtil::convertToBoolean(get(key), defaultValue);
}

double EZConfiguration::getDouble(const ::std::string& key, double defaultValue) const {
    return StringConversionUtil::convertToDouble(get(key), defaultValue);
}

float EZConfiguration::getFloat(const ::std::string& key, float defaultValue) const {
    return StringConversionUtil::convertToFloat(get(key), defaultValue);
}

int EZConfiguration::getInt(const ::std::string& key, int defaultValue) const {
    return StringConversionUtil::convertToInt(get(key), defaultValue);
}

long EZConfiguration::getLong(const ::std::string& key, long defaultValue) const {
    return StringConversionUtil::convertToLong(get(key), defaultValue);
}

::std::string EZConfiguration::getString(const ::std::string& key, const ::std::string& defaultValue) const {
    return get(key).get_value_or(defaultValue);
}

::std::string EZConfiguration::getPathString(const ::std::string& key, const ::std::string& defaultValue) const {
    ::boost::optional< ::std::string> path = get(key);
    if (!path) {
        return defaultValue;
    }

    if (!OpenShiftUtil::inOpenShiftContainer()) {
        return path.get();
    }

    return OpenShiftUtil::getRepoPath(path.get());
}

PropertiesUtil::Properties EZConfiguration::getProperties() const {
    return _properties;
}

void EZConfiguration::mergeWithProperties(const PropertiesUtil::Properties& toMergeWith, bool overrideDefaults) {
    PropertiesUtil::mergeProperties(_properties, toMergeWith, overrideDefaults);
}

void EZConfiguration::loadFromDirectory(const ::std::string& directory, bool shouldOverride) {
    PropertiesUtil::loadPropertiesFromDirectory(directory, _properties, shouldOverride);
}

void EZConfiguration::loadDefaultConfiguration() {
    LOG4CXX_INFO(LOG, "Attempting to load from default heirarchy...");
    if (!loadFromEZConfigurationVar()) {
        loadFromDirectory();
    }
    loadOpenShift();
}

bool EZConfiguration::loadFromEZConfigurationVar() {
    char * envPtr = std::getenv(EZCONFIGURATION_DIR_VAR.c_str());

    if(envPtr != NULL) {
        ::std::string defaultDir = envPtr;
        if (::std::string::npos != defaultDir.find_first_not_of(' ')) {
            try {
                PropertiesUtil::loadPropertiesFromDirectory(defaultDir, _properties);
                LOG4CXX_INFO(LOG, "Loaded properties form environment located at " + defaultDir);
                return true;

            } catch (const std::exception& e) {
                LOG4CXX_WARN(LOG, "Error in loading properties from " + defaultDir + ": " + e.what());
            }
        }
    } else {
        LOG4CXX_INFO(LOG, "Unable to load from EZConfigurationVar. Environment variable not set!");
    }

    return false;
}

void EZConfiguration::loadOpenShift() {
    if (OpenShiftUtil::inOpenShiftContainer()) {
        ::std::string configuratioDir = OpenShiftUtil::getConfigurationDir();
        try {
            PropertiesUtil::loadPropertiesFromDirectory(configuratioDir, _properties);
            LOG4CXX_INFO(LOG, "Loaded properties for Openshift located at " + configuratioDir);
        } catch (const std::exception& e) {
            LOG4CXX_WARN(LOG, "Error in loading properties from " + configuratioDir + ": " + e.what());
        }
    }
}

void EZConfiguration::loadFromDirectory() {
    PropertiesUtil::loadPropertiesFromDirectory(DEFAULT_EZCONFIGURATION_DIR, _properties);
    LOG4CXX_INFO(LOG, "Loaded Properties from default configuration located at" + DEFAULT_EZCONFIGURATION_DIR);
}

}} // namespace ::ezbake::configuration
