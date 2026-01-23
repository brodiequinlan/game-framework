/*
#include <bq.h>
#include <boost/test/included/unit_test.hpp>
BOOST_AUTO_TEST_CASE(getRandom) {
	constexpr float MIN_RANDOM = 50.0f;
	constexpr float MAX_RANDOM = 80.0f;
	constexpr unsigned NUM_CYCLES = 20000;
	for (unsigned i = 0; i < NUM_CYCLES; ++i) {
		float random = bq::random::getRandom(MIN_RANDOM, MAX_RANDOM);
		BOOST_CHECK(random >= MIN_RANDOM);
		BOOST_CHECK(random <= MAX_RANDOM);
	}
}
*/