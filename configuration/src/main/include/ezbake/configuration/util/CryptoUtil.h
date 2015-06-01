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

#ifndef EZBAKE_EZCONFIGURATION_UTIL_CRYPTOUTIL_H_
#define EZBAKE_EZCONFIGURATION_UTIL_CRYPTOUTIL_H_

#include <string>

namespace ezbake { namespace configuration { namespace util {

class CryptoUtil {
public:
    static ::std::string encode(const ::std::string& serializedData);
    static ::std::string decode(const ::std::string& encodedData);
    static ::std::string encode(const char * data, int length);
    static ::std::string decode(const char * data, int length);

private:
    CryptoUtil();
};

}}} // ::ezbake::configuration::util

#endif /* EZBAKE_EZCONFIGURATION_UTIL_CRYPTOUTIL_H_ */
