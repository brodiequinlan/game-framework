/*
#include <boost/test/included/unit_test.hpp>

#include <bq.h>


class test_item : public bq::item {
	// Inherited via item
	virtual void action(std::optional<sf::Keyboard::Key>, std::optional<bq::v2f>) override
	{
	}

};
BOOST_AUTO_TEST_CASE(test_empty_inventory) {
	bq::inventory inv;
	BOOST_REQUIRE(inv.empty());
}
BOOST_AUTO_TEST_CASE(not_empty_inventory) {
	bq::inventory inv;
	inv.add_item(std::make_unique<test_item>());
	BOOST_REQUIRE(!inv.empty());
}
BOOST_AUTO_TEST_CASE(cycle_inventory_forward) {
	auto ti1 = std::make_unique<test_item>();
	auto ti2 = std::make_unique<test_item>();
	test_item* ti1_ptr = ti1.get();
	test_item* ti2_ptr = ti2.get();
	
	bq::inventory inv;
	inv.add_item(std::move(ti1));
	inv.add_item(std::move(ti2));
	BOOST_REQUIRE_EQUAL(inv.get_selected(), ti1_ptr);
	BOOST_REQUIRE(inv.get_selected() != ti2_ptr);
	inv.cycle_forward();
	BOOST_REQUIRE_EQUAL(inv.get_selected(), ti2_ptr);
	BOOST_REQUIRE(inv.get_selected() != ti1_ptr);
}
BOOST_AUTO_TEST_CASE(test_delete_then_access) {
	bq::inventory inv;
	inv.add_item(std::make_unique<test_item>());
	inv.drop_item();
	BOOST_REQUIRE(inv.get_selected() == nullptr);

}
*/