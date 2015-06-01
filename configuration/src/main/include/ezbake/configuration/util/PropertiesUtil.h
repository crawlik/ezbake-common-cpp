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

#ifndef EZBAKE_EZCONFIGURATION_UTIL_PROPERTIESUTIL_H_
#define EZBAKE_EZCONFIGURATION_UTIL_PROPERTIESUTIL_H_

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <unordered_map>

namespace ezbake { namespace configuration { namespace util {

class PropertiesUtil {
public:
    typedef ::std::unordered_map< ::std::string, ::std::string> Properties;
    typedef ::std::pair< ::std::string, ::std::string> Property;

    static const ::std::string PROPERTY_FILE_EXTENSION;

public:
    static bool isEmpty(const Properties& props);
    static void checkForCollisions(const Properties& toCheckAgainst, const Properties& toCheck);
    static void mergeProperties(Properties& mergeWith, const Properties& toMerge, bool shouldOverride=true);
    static void loadPropertiesFromFile(const ::std::string& filename, Properties& loadInto, bool shouldOverride=true);
    static void loadPropertiesFromDirectory(const ::std::string& directory, Properties& loadInto, bool shouldOverride=true);
    static Properties getPropertiesByPrefix(const Properties& from, ::std::string prefix);

protected:
    static ::std::vector< ::std::string> stringPropertyNames(const Properties& props);
    static void loadPropertiesFromFileStream(std::ifstream& stream, Properties& loadInto, bool shouldOverride=true);

private:
    static Properties readPropertiesFromFileStream(std::ifstream& stream);

    PropertiesUtil();
};

}}} // namespace ::ezbake::configuration::util

#endif /* EZBAKE_EZCONFIGURATION_UTIL_PROPERTIESUTIL_H_ */
