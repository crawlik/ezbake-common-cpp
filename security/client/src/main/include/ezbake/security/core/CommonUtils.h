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


#ifndef EZBAKE_SECURITY_CORE_COMMONUTILS_H_
#define EZBAKE_SECURITY_CORE_COMMONUTILS_H_

#include <chrono>

namespace ezbake { namespace security { namespace core { namespace CommonUtils {

/**
 * Returns the current time in milliseconds
 */
inline int64_t currentTimeMillis() {
    return static_cast<int64_t>(::std::chrono::system_clock::now().time_since_epoch() / ::std::chrono::milliseconds(1));
}

}}}} // namespace ezbake::security::core::CommonUtils

#endif /* EZBAKE_SECURITY_CORE_COMMONUTILS_H_ */
