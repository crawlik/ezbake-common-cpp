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

#ifndef EZBAKE_EZCONFIGURATION_EZCONFIGURATION_H_
#define EZBAKE_EZCONFIGURATION_EZCONFIGURATION_H_

#include <boost/optional.hpp>

#include "ezbake/configuration/util/PropertiesUtil.h"

namespace ezbake { namespace configuration {

class EZConfiguration {
public:
    static const ::std::string DEFAULT_EZCONFIGURATION_DIR;
    static const ::std::string EZCONFIGURATION_DIR_VAR;

public:
    EZConfiguration(bool loadDefaults=true);
    EZConfiguration(const util::PropertiesUtil::Properties& props);

    virtual ~EZConfiguration();

    ::boost::optional< ::std::string> get(const ::std::string& key) const;
    void set(const ::std::string& key, const ::std::string& value, bool shouldOverride=false);

    //Conversion functions
    bool getBoolean(const ::std::string& key, bool defaultValue) const;
    double getDouble(const ::std::string& key, double defaultValue) const;
    float getFloat(const ::std::string& key, float defaultValue) const;
    int getInt(const ::std::string& key, int defaultValue) const;
    long getLong(const ::std::string& key, long defaultValue) const;
    ::std::string getString(const ::std::string& key, const ::std::string& defaultValue) const;
    ::std::string getPathString(const ::std::string& key, const ::std::string& defaultValue) const;

    //Utility functions
    util::PropertiesUtil::Properties getProperties() const;
    void mergeWithProperties(const util::PropertiesUtil::Properties& toMergeWith, bool overrideDefaults=false);
    void loadFromDirectory(const ::std::string& directory, bool shouldOverride=true);

private:
    void loadDefaultConfiguration();
    bool loadFromEZConfigurationVar();
    void loadOpenShift();
    void loadFromDirectory();

private:
    util::PropertiesUtil::Properties _properties;
};

}} // namespace ::ezbake::configuration

#endif /* EZBAKE_EZCONFIGURATION_EZCONFIGURATION_H_ */
