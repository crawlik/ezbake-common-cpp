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
#include <stdexcept>
#include <ezbake/configuration/EZConfiguration.h>


class EZConfigurationTest : public ::testing::Test {
public:
    EZConfigurationTest() {}
    virtual ~EZConfigurationTest() {}
};


using namespace ::ezbake::configuration;


TEST_F(EZConfigurationTest, LoadFromDefaults) {
    EZConfiguration config;
    ASSERT_EQ("testapp", config.getString("application.name", "fauxApp"));
}

TEST_F(EZConfigurationTest, LoadFromCopy) {
    util::PropertiesUtil::Properties props({{"hello", "world"}, {"good", "bye"}});
    EZConfiguration config(props);

    EXPECT_EQ(props.size(), config.getProperties().size());
    EXPECT_EQ(props.at("hello"), config.getString("hello", "FAILED"));
    EXPECT_EQ(props.at("good"), config.getString("good", "FAILED"));
}

TEST_F(EZConfigurationTest, SetAndGet) {
    EZConfiguration config(false);

    config.set("TestKey", "TestValue");
    EXPECT_EQ("TestValue", config.get("TestKey").get());

    //expect false since optional value will not be set for invalid key
    EXPECT_FALSE(config.get("InvalidKey"));
}

TEST_F(EZConfigurationTest, PropertyAlreadySet) {
    EZConfiguration config(false);

    config.set("TestKey", "TestValue");
    EXPECT_THROW(config.set("TestKey", "DuplicateTestValue"), std::runtime_error);
}

TEST_F(EZConfigurationTest, PropertiesDeepCopy) {
    util::PropertiesUtil::Properties props;
    EZConfiguration config1(false);

    config1.set("TestKey", "TestValue");
    props = config1.getProperties();
    ASSERT_EQ(static_cast<unsigned int>(1), props.size());

    props["TestKey"] = "AnotherValue";
    EXPECT_EQ("TestValue", config1.get("TestKey").get());

    EZConfiguration config2(config1);
    EXPECT_EQ("TestValue", config2.get("TestKey").get());
}

TEST_F(EZConfigurationTest, GetPropertyNotPreviouslySet) {
    EZConfiguration config(false);
    ASSERT_FALSE(config.get("TestKey"));
}

TEST_F(EZConfigurationTest, GetBoolean) {
    util::PropertiesUtil::Properties props({{"Key", "True"}, {"SecondKey", "false"}});
    EZConfiguration config(props);

    EXPECT_TRUE(config.getBoolean("Key", false));
    EXPECT_FALSE(config.getBoolean("SecondKey", true));
    EXPECT_FALSE(config.getBoolean("NonExisitentKey", false));
}

TEST_F(EZConfigurationTest, GetDouble) {
    util::PropertiesUtil::Properties props({{"Key", "2.3333333333"}});
    EZConfiguration config(props);

    EXPECT_DOUBLE_EQ(2.3333333333, config.getDouble("Key", 1.1));
    EXPECT_DOUBLE_EQ(3.141415926535, config.getDouble("NonExistentKey", 3.141415926535));
}

TEST_F(EZConfigurationTest, GetFloat) {
    util::PropertiesUtil::Properties props({{"Key", "2.3"}});
    EZConfiguration config(props);

    EXPECT_FLOAT_EQ(2.3f, config.getFloat("Key", 1.1f));
    EXPECT_FLOAT_EQ(3.14f, config.getFloat("NonExistentKey", 3.14f));
}

TEST_F(EZConfigurationTest, GetInt) {
    util::PropertiesUtil::Properties props({{"Key", "12"}});
    EZConfiguration config(props);

    EXPECT_EQ(12, config.getInt("Key", 0));
    EXPECT_EQ(34, config.getInt("NonExistentKey", 34));
}

TEST_F(EZConfigurationTest, GetLong) {
    util::PropertiesUtil::Properties props({{"Key", "29381984930"}});
    EZConfiguration config(props);

    EXPECT_EQ(29381984930, config.getLong("Key", 1000000000));
    EXPECT_EQ(1000000000, config.getLong("NonExistentKey", 1000000000));
}

TEST_F(EZConfigurationTest, GetString) {
    util::PropertiesUtil::Properties props({{"Key", "Value"}});
    EZConfiguration config(props);

    EXPECT_EQ("Value", config.getString("Key", "WrongValue"));
    EXPECT_EQ("DefaultValue", config.getString("NonExistentKey", "DefaultValue"));
}
