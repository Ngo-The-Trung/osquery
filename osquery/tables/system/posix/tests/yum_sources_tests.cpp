/**
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under both the Apache 2.0 license (found in the
 *  LICENSE file in the root directory of this source tree) and the GPLv2 (found
 *  in the COPYING file in the root directory of this source tree).
 *  You may select, at your option, one of the above-listed licenses.
 */

#include<sstream>

#include <gtest/gtest.h>

#include <osquery/sql.h>

namespace osquery {
namespace tables {

void parseYumConf(std::istream&, QueryData&ults, std::string&epos_dir);

class YumSourcesTests : public testing::Test {};

TEST_F(YumSourcesTests, parse_yum_conf) {
  QueryData results;
  std::string repos_dir;

  std::istringstream stream1("");
  parseYumConf(stream1, results, repos_dir);
  ASSERT_EQ(repos_dir, "/etc/yum.repos.d");
  ASSERT_EQ(results.size(), (unsigned long) 0);

  std::istringstream stream2(R"STRLIT(
# Some comment

[main]
cachedir=/var/cache/yum
reposdir=/etc/local/yum.repos.d

[personal]
baseurl=http://my.repo.url/1/v2/3
enabled=1
name=My personal repo
reposdir=/ignored/path

[math]
baseurl=http://some.math.repo.url
enabled=0
name=Mathematic library repo
gpgcheck=0
)STRLIT");

  parseYumConf(stream2, results, repos_dir);
  ASSERT_EQ(repos_dir, "/etc/local/yum.repos.d");
  ASSERT_EQ(results.size(), (unsigned long) 2);
  ASSERT_EQ(results.at(0).at("baseurl"), "http://my.repo.url/1/v2/3");
  ASSERT_EQ(results.at(0).at("enabled"), "1");
  ASSERT_EQ(results.at(0).at("name"), "My personal repo");
  ASSERT_EQ(results.at(0).find("gpgcheck"), results.at(0).end());
  ASSERT_EQ(results.at(1).at("baseurl"), "http://some.math.repo.url");
  ASSERT_EQ(results.at(1).at("enabled"), "0");
  ASSERT_EQ(results.at(1).at("name"), "Mathematic library repo");
  ASSERT_EQ(results.at(1).at("gpgcheck"), "0");
}

} // namespace tables
} // namespace osquery
