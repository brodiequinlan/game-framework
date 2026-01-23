#include "bqpch.h"
#include <bq/graphics/sprite.h>
#include "bq/graphics/window.h"
#include "bq/core/handler.h"

#include <bq/state/state.h>
#include <bq/entity/entity.h>

bq::window::window(unsigned w, unsigned h,const std::string& title):m_window({ w,h }, title, sf::Style::Default) {}

bq::v2f intl_get_mouse_pos(sf::Event& evt) {
	bq::v2f offset = { 0,0 };
	if (bq::handler::get().has_cam()) {
		offset = { bq::handler::get().cam()->view().getCenter().x - (bq::handler::get().cam()->view().getSize().x / 2)   , bq::handler::get().cam()->view().getCenter().y - (bq::handler::get().cam()->view().getSize().y / 2) };
	}
	bq::v2f pos = { (float)evt.mouseButton.x + offset.x, (float)evt.mouseButton.y + offset.y };
	return pos;
}

bq::keyboard::keycode intl_convert_keycode(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::W:
		return bq::keyboard::keycode::W;
		break;
	case sf::Keyboard::A:
		return bq::keyboard::keycode::A;
		break;
	case sf::Keyboard::S:
		return bq::keyboard::keycode::S;
		break;
	case sf::Keyboard::D:
		return bq::keyboard::keycode::D;
		break;
	case sf::Keyboard::Up:
		return bq::keyboard::keycode::UP;
		break;
	case sf::Keyboard::P:
		return bq::keyboard::keycode::P;
		break;
	case sf::Keyboard::F:
		return bq::keyboard::keycode::F;
		break;
	case sf::Keyboard::Left:
		return bq::keyboard::keycode::LEFT;
		break;
	case sf::Keyboard::Down:
		return bq::keyboard::keycode::DOWN;
		break;
	case sf::Keyboard::Right:
		return bq::keyboard::keycode::RIGHT;
		break;
	case sf::Keyboard::Enter:
		return bq::keyboard::keycode::ENTER;
		break;
	case sf::Keyboard::Tab:
		return bq::keyboard::keycode::TAB;
		break;
	case sf::Keyboard::Space:
		return bq::keyboard::keycode::SPACE;
		break;

	default:
		return bq::keyboard::keycode::NONE;
		break;
	}
}
void bq::window::show() {
	m_window.display();
}
void bq::window::close() {
	m_window.close();
}

void bq::window::set_view(const sf::View& view)
{
	m_window.setView(view);
}

void bq::window::draw(const sf::RectangleShape& r)
{
	m_window.draw(r);
}

void bq::window::draw(const sf::Text& t)
{
	m_window.draw(t);
}


bq::v2i bq::window::get_mouse_pos()
{
	return { sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y };
}

void bq::window::draw(const bq::sprite& sprite) {
	m_window.draw(sprite.m_sprite);
}
bool bq::window::open() {
	return m_window.isOpen();
}
bq::event bq::window::poll_event() {
	sf::Event sf_event;
	if (m_window.pollEvent(sf_event)) {
		switch (sf_event.type) {
		case sf::Event::Closed:
			return bq::event(bq::event_type::CLOSE);
			break;
		case sf::Event::KeyPressed:
			return bq::event(bq::event_type::KEYPRESSED, intl_convert_keycode(sf_event.key.code));
			break;
		case sf::Event::KeyReleased:
			return bq::event(bq::event_type::KEYRELEASED, intl_convert_keycode(sf_event.key.code));
			break;
		case sf::Event::MouseButtonPressed:
			return bq::event(bq::event_type::MOUSE, intl_get_mouse_pos(sf_event));
			break;
		default:
			return bq::event(bq::event_type::NONE);
			break;
		}
	}
	return bq::event(bq::event_type::NONE);
}

void bq::window::clear(colour c)
{
	switch (c) {
	case colour::BLACK:
		m_window.clear(sf::Color::Black);
	}
}
