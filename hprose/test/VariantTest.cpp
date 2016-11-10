/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * VariantTest.cpp                                        *
 *                                                        *
 * hprose variant test for cpp.                           *
 *                                                        *
 * LastModified: Nov 10, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/Variant.h>

#include <gtest/gtest.h>

TEST(Variant, Default) {
  hprose::Variant var;
  EXPECT_TRUE(var.isNull());
}