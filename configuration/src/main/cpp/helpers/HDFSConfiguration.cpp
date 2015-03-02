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

#include "ezbake/configuration/helpers/HDFSConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<HDFSConfiguration> HDFSConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<HDFSConfiguration> p(
        new HDFSConfiguration(configuration, namespacePrefix));

    return p;
}

HDFSConfiguration::HDFSConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

HDFSConfiguration::~HDFSConfiguration() {
}

::boost::optional< ::std::string> HDFSConfiguration::getFileSystemName() const {
    return get(g_EzBakeProperty_constants.HADOOP_FILESYSTEM_NAME);
}

::std::string HDFSConfiguration::getFileSystemNameWithDefault() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.HADOOP_FILESYSTEM_NAME),
        "hdfs://127.0.0.1:8020");
}

bool HDFSConfiguration::getUseLocal() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.HADOOP_FILESYSTEM_USE_LOCAL),
        false);
}

::std::string HDFSConfiguration::getFileSystemImpl() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.HADOOP_FILESYSTEM_IMPL),
        "org.apache.hadoop.hdfs.DistributedFileSystem");
}

}}} // namespace ::ezbake::configuration::helpers
