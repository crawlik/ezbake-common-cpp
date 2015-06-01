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

#include "ezbake/configuration/helpers/KafkaConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<KafkaConfiguration> KafkaConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<KafkaConfiguration> p(
        new KafkaConfiguration(configuration, namespacePrefix));

    return p;
}

KafkaConfiguration::KafkaConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

KafkaConfiguration::~KafkaConfiguration() {
}

::std::string KafkaConfiguration::getZookeepers() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.KAFKA_ZOOKEEPER),
        "");
}

::std::string KafkaConfiguration::getBrokerList() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.KAFKA_BROKER_LIST),
        "");
}

::std::string KafkaConfiguration::getConsumerTimeout() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.KAFKA_ZOOKEEPER_SESSION_TIMEOUT),
        "");
}

}}} //namespace ::ezbake::configuration::helpers
