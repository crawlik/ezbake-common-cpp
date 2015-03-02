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

#ifndef EZBAKE_EZCONFIGURATION_UTIL_OPENSHIFTUTIL_H_
#define EZBAKE_EZCONFIGURATION_UTIL_OPENSHIFTUTIL_H_

#include "ezbake/configuration/util/HostAndPort.h"

namespace ezbake { namespace configuration { namespace util {

class OpenShiftUtil {
public:
    static const ::std::string FILE_SEPARATOR;
    static const ::std::string OPENSHIFT_REPO_DIR;
    static const ::std::string OPENSHIFT_JAVA_THRIFTRUNNER_IP;
    static const ::std::string OPENSHIFT_JAVA_THRIFTRUNNER_TCP_PORT;

public:
    static ::std::string getConfigurationDir();
    static bool inOpenShiftContainer();
    static ::std::string getRepoDir();
    static ::std::string getRepoPath(const ::std::string& file);
    static HostAndPort getThriftPrivateInfo();

private:
    OpenShiftUtil();
};

}}} // namespace ::ezbake::configuration::util

#endif /* EZBAKE_EZCONFIGURATION_UTIL_OPENSHIFTUTIL_H_ */
