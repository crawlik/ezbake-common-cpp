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

#include "ezbake/configuration/helpers/ElasticsearchConfiguration.h"

#include "EzBakeProperty_constants.h"

using ezbake::configuration::constants::g_EzBakeProperty_constants;

namespace ezbake { namespace configuration { namespace helpers {

::boost::shared_ptr<ElasticsearchConfiguration> ElasticsearchConfiguration::fromConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix) {
    ::boost::shared_ptr<ElasticsearchConfiguration> p(
        new ElasticsearchConfiguration(configuration, namespacePrefix));

    return p;
}

ElasticsearchConfiguration::ElasticsearchConfiguration(
        const ::ezbake::configuration::EZConfiguration& configuration,
        const ::std::string& namespacePrefix)
    : BaseConfiguration(configuration, namespacePrefix) {
}

ElasticsearchConfiguration::~ElasticsearchConfiguration() {
}

::std::string ElasticsearchConfiguration::getElasticsearchHost() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ELASTICSEARCH_HOST),
        "");
}

int ElasticsearchConfiguration::getElasticsearchPort() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.ELASTICSEARCH_PORT),
        9200);
}

int ElasticsearchConfiguration::getElasticsearchThriftPort() const {
    return ezConfiguration.getInt(
        parseKey(g_EzBakeProperty_constants.ELASTICSEARCH_THRIFT_PORT),
        9500);
}

::std::string ElasticsearchConfiguration::getElasticsearchClusterName() const {
    return ezConfiguration.getString(
        parseKey(g_EzBakeProperty_constants.ELASTICSEARCH_CLUSTER_NAME),
        "");
}

bool ElasticsearchConfiguration::getForceRefresh() const {
    return ezConfiguration.getBoolean(
        parseKey(g_EzBakeProperty_constants.ELASTICSEARCH_FORCE_REFRESH_ON_PUT),
        false);
}

}}} // namespace ::ezbake::configuration::helpers
