/*
#include <boost/test/included/unit_test.hpp>
#include <bq.h>

BOOST_AUTO_TEST_CASE(test_success_http_get){
    //THIS IS SIMPLY THE FIRST PUBLIC, API-KEYLESS, HTTP (NOT HTTPS) API I FOUND, I HAVE NO ASSOCIATION WITH THIS AT ALL
    bq::http_client httpc {"http://calapi.inadiutorium.cz"};
    bq::network_result result = httpc.get("/api/v0/en/calendars/default/2015/6/4");
    BOOST_CHECK(result.status == 200);
    bq::json j = bq::json::parse(result.body);
    BOOST_CHECK(j.get_i("season_week") == 9);
    BOOST_CHECK(j.get_s("season") == "ordinary");
}
BOOST_AUTO_TEST_CASE(test_success_https_get){
    //THIS IS SIMPLY THE FIRST PUBLIC, API-KEYLESS, HTTP (NOT HTTPS) API I FOUND, I HAVE NO ASSOCIATION WITH THIS AT ALL
    bq::http_client httpc {"https://www.google.com"};
    bq::network_result result = httpc.get("/");
    BOOST_CHECK(result.status == 200);
}
BOOST_AUTO_TEST_CASE(test_fail_https_get){
    //THIS IS SIMPLY THE FIRST PUBLIC, API-KEYLESS, HTTP (NOT HTTPS) API I FOUND, I HAVE NO ASSOCIATION WITH THIS AT ALL
    bq::http_client httpc {"https://www.f45a4qweRRf6.com"};
    bq::network_result result = httpc.get("/");
    BOOST_CHECK(result.status == bq::BQ_NETWORK_FAILURE);
}
*/