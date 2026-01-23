/*
#include <boost/test/included/unit_test.hpp>

#include <bq.h>

#include <bq/util/json.h>

BOOST_AUTO_TEST_CASE(json_parser_test_string) {
    bq::json j = bq::json::parse("{\"key\": \"123\"");
    BOOST_REQUIRE(j.get_s("key") == "123");
}
BOOST_AUTO_TEST_CASE(json_parser_test_int) {
    bq::json j = bq::json::parse("{\"key\": 123");
    BOOST_REQUIRE(j.get_i("key") == 123);
}
BOOST_AUTO_TEST_CASE(json_parser_test_bool) {
    bq::json j = bq::json::parse("{\"key\": true");
    BOOST_REQUIRE(j.get_b("key") == true);
}
BOOST_AUTO_TEST_CASE(json_parser_test_not_equal) {
    bq::json j = bq::json::parse("{\"key\": 123");
    BOOST_REQUIRE(! (j.get_i("key") == 124));
    j = bq::json::parse("{\"key\": \"123\"");
    BOOST_REQUIRE(! (j.get_s("key") == "124"));
    j = bq::json::parse("{\"key\": false");
    BOOST_REQUIRE(! (j.get_b("key") == true));
}
BOOST_AUTO_TEST_CASE(json_parser_test_multiple_add) {
    bq::json j = bq::json::parse("{\"key\": \"123\",\"key2\": 123 \"key3\": false");
    BOOST_REQUIRE(j.get_s("key") == "123");
    BOOST_REQUIRE(j.get_i("key2") == 123);
    BOOST_REQUIRE(j.get_b("key3") == false);
}
BOOST_AUTO_TEST_CASE(json_parser_test_sub_object) {
    bq::json j = bq::json::parse("{\"key\": { \"subkey\": 123,\"poop\" : \"butt\" }, \"key2\" : 2 ");
    BOOST_REQUIRE(j.get_o("key").get_i("subkey") == 123);
    BOOST_REQUIRE(j.get_o("key").get_s("poop") == "butt");
    BOOST_REQUIRE(j.get_i("key2") == 2);
}

BOOST_AUTO_TEST_CASE(json_parser_test_multiple_nests) {
    bq::json j = bq::json::parse("{\"key\": { \"sub\":{ \"subkey\": 123,\"poop\" : \"butt\" } }");
    BOOST_REQUIRE(j.get_o("key").get_o("sub").get_i("subkey") == 123);
}

BOOST_AUTO_TEST_CASE(json_parser_test_arrays) {
    bq::json j = bq::json::parse("{\"key\":[1,3,4,5,6,7,8,3 ]}");
    BOOST_REQUIRE(j.get_a("key").get_i(0) == 1);
}
BOOST_AUTO_TEST_CASE(json_parser_test_arrays_and_strings) {
    bq::json j = bq::json::parse("{\"key\": [ \"pop\",\"pop\",\"pop2\",\"pop\"], \"key2\": \"poop\" }");

    BOOST_REQUIRE(j.get_a("key").get_s(2) == "pop2");
    BOOST_REQUIRE(j.get_s("key2") == "poop");
}
BOOST_AUTO_TEST_CASE(json_parser_test_arrays_and_objects) {
    bq::json j = bq::json::parse("{\"key\": { \"obj\" : [ \"pop\",\"pop\",\"pop2\",\"pop\"]}, \"key2\": \"poop\"}");

    BOOST_REQUIRE(j.get_o("key").get_a("obj").get_s(2) == "pop2");
    BOOST_REQUIRE(j.get_s("key2") == "poop");
}
*/