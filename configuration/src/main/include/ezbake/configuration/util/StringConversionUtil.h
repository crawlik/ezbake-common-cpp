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

#ifndef EZBAKE_EZCONFIGURATION_UTIL_STRINGCONVERSIONUTIL_H_
#define EZBAKE_EZCONFIGURATION_UTIL_STRINGCONVERSIONUTIL_H_

#include <string>

#include <boost/optional.hpp>

namespace ezbake { namespace configuration { namespace util {

class StringConversionUtil
{
public:
    static bool convertToBoolean(const ::boost::optional< ::std::string>& toConvert, bool defaultValue);
    static double convertToDouble(const ::boost::optional< ::std::string>& toConvert, double defaultValue);
    static float convertToFloat(const ::boost::optional< ::std::string>& toConvert, float defaultValue);
    static int convertToInt(const ::boost::optional< ::std::string>& toConvert, int defaultValue);
    static long convertToLong(const ::boost::optional< ::std::string>& toConvert, long defaultValue);

private:
    StringConversionUtil();
};

}}} //namespace ::ezbake::configuration::util

#endif /* EZBAKE_EZCONFIGURATION_UTIL_STRINGCONVERSIONUTIL_H_ */
