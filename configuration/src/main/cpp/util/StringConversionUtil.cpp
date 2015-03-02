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

#include "ezbake/configuration/util/StringConversionUtil.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>

namespace ezbake { namespace configuration { namespace util {

bool StringConversionUtil::convertToBoolean(const ::boost::optional< ::std::string>& toConvert, bool defaultValue) {
    if (!toConvert) {
        return defaultValue;
    }

    ::std::string transform = toConvert.get();
    ::std::transform(transform.begin(), transform.end(), transform.begin(), ::tolower);

    if ((transform == "true") || (transform == "1") || (transform == "t") ||
        (transform == "y") || (transform == "yes")) { return true; }
    return false;
}

double StringConversionUtil::convertToDouble(const ::boost::optional< ::std::string>& toConvert, double defaultValue) {
    if (!toConvert) {
        return defaultValue;
    }

    return ::boost::lexical_cast<double>(toConvert.get());
}

float StringConversionUtil::convertToFloat(const ::boost::optional< ::std::string>& toConvert, float defaultValue) {
    if (!toConvert) {
        return defaultValue;
    }

    return ::boost::lexical_cast<float>(toConvert.get());
}

int StringConversionUtil::convertToInt(const ::boost::optional< ::std::string>& toConvert, int defaultValue) {
    if (!toConvert) {
        return defaultValue;
    }

    return ::boost::lexical_cast<int>(toConvert.get());
}

long StringConversionUtil::convertToLong(const ::boost::optional< ::std::string>& toConvert, long defaultValue) {
    if (!toConvert) {
        return defaultValue;
    }

    return ::boost::lexical_cast<long>(toConvert.get());
}

}}} //namespace ::ezbake::configuration::util
