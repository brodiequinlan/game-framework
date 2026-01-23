#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <bq.h>


class test_entity : public bq::entity {
public:
	test_entity() {
		m_pos.x = 400;
		m_pos.y = 400;
	}
	void update() override {
		m_pos += bq::v2f(1,1);
	}
	void render(bq::window&) override {}
	void handle_event(bq::event&)   override {}
	void damage(float) override {}
	void interact() override {}
	bool should_cull(const sf::View&) const override {return false;}
	virtual ~test_entity() override {}
};
TEST_CASE("test_camera_update") {

	bq::entity* e = new test_entity();
	bq::camera cam(e);
	bq::v2f pos(GAME_WIDTH, GAME_HEIGHT);
	CHECK(cam.view().getSize().x == pos.x);
	CHECK(cam.view().getSize().y == pos.y);
	delete e;
}
TEST_CASE("test_camera_update") {
	bq::entity* e = new test_entity();
	bq::camera cam(e);

	for (unsigned i = 0; i < 100; ++i) {
		e->update();
		cam.update();
		CHECK(cam.view().getCenter().x == e->pos().x);
		CHECK(cam.view().getCenter().y == e->pos().y);
	}
	delete e;
}