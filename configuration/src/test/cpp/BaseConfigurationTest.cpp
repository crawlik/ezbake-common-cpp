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


#include "AllTests.h"
#include <ezbake/configuration/helpers/BaseConfiguration.h>


class BaseConfigurationTest : public ::testing::Test {
public:
    BaseConfigurationTest() {}
    virtual ~BaseConfigurationTest() {}
};


using namespace ezbake::configuration;
using namespace ezbake::configuration::helpers;


TEST_F(BaseConfigurationTest, WithoutNamspacePrefixing) {
    EZConfiguration configuration;
    BaseConfiguration config(configuration);

    ASSERT_TRUE(config.get("application.name"));
    EXPECT_EQ("testapp", config.get("application.name").get());
}

TEST_F(BaseConfigurationTest, WithNamspacePrefixing) {
    EZConfiguration configuration;
    BaseConfiguration config(configuration, "TestNamespace");

    ASSERT_TRUE(config.get("accumulo.username"));
    EXPECT_EQ("TestUserWithNamespace", config.get("accumulo.username").get());
}
