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

#include "ezbake/configuration/helpers/ZookeeperConfiguration.h"
#include "ezbake/configuration/helpers/SecurityConfiguration.h"
#include "ezbake/configuration/helpers/ApplicationConfiguration.h"
#include "ezbake/configuration/helpers/ThriftConfiguration.h"
#include "ezbake/configuration/helpers/PostgresConfiguration.h"

class HelpersConfigurationTest : public ::testing::Test {
public:
    HelpersConfigurationTest() {}
    virtual ~HelpersConfigurationTest() {}
};


using namespace ezbake::configuration;
using namespace ezbake::configuration::helpers;


TEST_F(HelpersConfigurationTest, ZooKeeperConfiguration) {
    EZConfiguration config;
    ::boost::shared_ptr<ZookeeperConfiguration> zk = ZookeeperConfiguration::fromConfiguration(config);
    EXPECT_EQ("zoo1:2181,zoo2:2181", zk->getZookeeperConnectionString());
}

TEST_F(HelpersConfigurationTest, SecurityConfiguration) {
    EZConfiguration config;
    ::boost::shared_ptr<SecurityConfiguration> configuration = SecurityConfiguration::fromConfiguration(config);
    ::boost::shared_ptr<ApplicationConfiguration> appConfig = ApplicationConfiguration::fromConfiguration(config);


    EXPECT_EQ(SecurityConfiguration::DEFAULT_SSL_PROTOCOLS, configuration->getSslProtocols());
    EXPECT_EQ(SecurityConfiguration::DEFAULT_SSL_CIPHERS, configuration->getSslCiphers());
    EXPECT_TRUE(configuration->isPeerAuthRequired());

    EXPECT_EQ(appConfig->getCertificatesDir() + "/" + SecurityConfiguration::DEFAULT_PRIVATE_KEY,
              configuration->getPrivateKey());
    EXPECT_EQ(appConfig->getCertificatesDir() + "/" + SecurityConfiguration::DEFAULT_SERVICE_PUBLIC_KEY,
              configuration->getServicePublicKey());
    EXPECT_EQ(appConfig->getCertificatesDir() + "/" + SecurityConfiguration::DEFAULT_SSL_CERT,
              configuration->getSslCertificate());
    EXPECT_EQ(appConfig->getCertificatesDir() + "/" + SecurityConfiguration::DEFAULT_TURSTED_SSL_CERTS,
              configuration->getTrustedSslCerts());

    ASSERT_EQ("../../../src/test/resources/ssl/test.priv", configuration->getPrivateKey("testService"));
    ASSERT_EQ("../../../src/test/resources/ssl/test.pub", configuration->getServicePublicKey("testService"));

    EXPECT_NO_THROW(configuration->setPrivateKey("testService"));
    EXPECT_NO_THROW(configuration->setPublicKey("testService"));
    EXPECT_TRUE(configuration->hasPrivateKeySet());
    EXPECT_TRUE(configuration->hasPublicKeySet());
    ASSERT_TRUE(configuration->hasKeysSet());

    std::string data = "HelloWorld\nThis is:a {new} \"Day\"\n\n";
    EXPECT_TRUE(configuration->verify(data, configuration->sign(data)));
}

TEST_F(HelpersConfigurationTest, ThriftConfiguration) {
    EZConfiguration config;
    ::boost::shared_ptr<ThriftConfiguration> configuration = ThriftConfiguration::fromConfiguration(config);
    EXPECT_FALSE(configuration->useSSL());
    EXPECT_EQ(ThriftConfiguration::HsHa, configuration->getServerMode());
}

TEST_F(HelpersConfigurationTest, PostgresConfiguration) {
    EZConfiguration config;
    ::boost::shared_ptr<PostgresConfiguration> pg = PostgresConfiguration::fromConfiguration(config);
    EXPECT_NO_THROW(pg->getConnectionUrl());
}
