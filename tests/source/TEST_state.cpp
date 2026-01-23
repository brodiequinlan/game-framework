
/*
#include <boost/test/included/unit_test.hpp>
#include <bq.h>
#include <bq/exception/illegal_state.h>
#include <bq/state/state.h>
#include <bq/state/state_manager.h>


class test_state : public bq::state {
public:
	void update() override{

	}
	virtual ~test_state() {

	}

	// Inherited via state
	virtual void render(bq::window& window) override
	{
	}
	virtual void handleEvents(bq::event& evt, bq::window& window) override
	{
	}
};
BOOST_AUTO_TEST_CASE(test_state_manager_constructor) {
	bq::state_manager sm(std::make_unique<test_state>());
	BOOST_CHECK(sm.get_num_states() == 1);
}
BOOST_AUTO_TEST_CASE(test_state_manager_pop) {
	bq::state_manager sm(std::make_unique<test_state>());
	sm.pop();
	BOOST_CHECK(sm.get_num_states() == 0);
}
BOOST_AUTO_TEST_CASE(test_state_manager_replace_true) {

	auto ts = std::make_unique<test_state>();
	auto ts1 = std::make_unique<test_state>();

	test_state* ts_ptr = ts.get();
	test_state* ts1_ptr = ts1.get();

	bq::state_manager sm(std::move(ts));
	sm.push(std::move(ts1),true);
	BOOST_CHECK(sm.get_num_states() == 1);

	BOOST_CHECK(sm.get_current_state() == ts1_ptr);
	BOOST_CHECK(sm.get_current_state() != ts_ptr);
}
BOOST_AUTO_TEST_CASE(test_state_manager_replace_false) {
	auto ts = std::make_unique<test_state>();
	auto ts1 = std::make_unique<test_state>();

	test_state* ts_ptr = ts.get();
	test_state* ts1_ptr = ts1.get();
	
	bq::state_manager sm(std::move(ts));
	sm.push(std::move(ts1), false);
	BOOST_CHECK(sm.get_num_states() == 2);

	BOOST_CHECK(sm.get_current_state() == ts1_ptr);
	sm.pop();
	BOOST_CHECK(sm.get_current_state() == ts_ptr);
}
BOOST_AUTO_TEST_CASE(test_state_manager_empty) {

	bq::state_manager sm(std::make_unique<test_state>());
	sm.pop();
	BOOST_REQUIRE_THROW(sm.update(), bq::illegal_state);
	bq::window window(4,4,"title");
	BOOST_REQUIRE_THROW(sm.render(window), bq::illegal_state);
}
*/