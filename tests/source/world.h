#pragma once



class test_block : public bq::block {
public:
	test_block(float x, float y, float l, float w) {
		m_pos = { x,y };
		m_size = { l,w };
	}

	bool contains(float x, float y) override {
		sf::FloatRect rect = { m_pos.x, m_pos.y, m_size.x, m_size.y };
		return rect.contains({ x,y });
	}
	void update() override {

	}
	void interact() override {

	}

	// Inherited via block
	virtual void render(bq::window&) override
	{
	}

};
class test_block2 : public bq::block {
public:
	test_block2(float x, float y, float l, float w) {
		m_pos = { x,y };
		m_size = { l,w };
	}
	bool contains(float x, float y) override {
		sf::FloatRect rect = { m_pos.x, m_pos.y, m_size.x, m_size.y };
		return rect.contains({ x,y });
	}
	void update() override {

	}
	void render(bq::window& window) override {

	}
	void interact() override {

	}
};


class test_world : public bq::world {
public:
	test_world() {
		m_blocks.push_back(std::make_shared<test_block>(32, 32, 32, 32));
	}
	virtual std::unique_ptr<std::vector<bq::v2i>> get_path(bq::v2i, bq::v2i) override {
		return std::make_unique<std::vector<bq::v2i>>();

	}
};