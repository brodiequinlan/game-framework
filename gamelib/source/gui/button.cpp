#include "bqpch.h"

#include <bq/event/event.h>
#include <bq/entity/entity.h>
#include <bq/gui/button.h>
#include <bq/core/handler.h>
#include <bq/resource/resource_holder.h>
#include <bq/state/state.h>
#include <bq/graphics/window.h>
void bq::gui::Button::handle_event(bq::event& e) {
	if (e.type == bq::event_type::MOUSE) {
		if (m_button.getGlobalBounds().contains(e.mouse_clicked_pos)) {
			bq::logger::debug("Button pressed");
			m_function();
		}
	}
}

void bq::gui::Button::set_text(const std::string& text)
{
	m_text.setString(text);
}

void bq::gui::Button::render(bq::window& window)
{
	bq::v2f offset = { 0,0 };

	if (bq::handler::get().has_cam()) {
		offset = { bq::handler::get().cam()->view().getCenter().x - (bq::handler::get().cam()->view().getSize().x / 2)   , bq::handler::get().cam()->view().getCenter().y - (bq::handler::get().cam()->view().getSize().y / 2) };
	}
	

	auto pos = window.get_mouse_pos();
	if (m_button.getGlobalBounds().contains((float)pos.x+offset.x, (float)pos.y + offset.y)) {
		m_button.setFillColor(sf::Color::Magenta);
	}
	else {
		m_button.setFillColor(m_fill);
	}

	window.draw(m_button);
	window.draw(m_text);
}

void bq::gui::Button::update()
{
}
void bq::gui::Button::setFunc(std::function<void(void)>func)
{
	m_function = func;
}

bq::gui::Button::Button(bq::v2f pos, bq::v2f size,std::string label, sf::Color outlineColor, sf::Color fillColor, unsigned textSize):m_fill(fillColor) {
	m_button.setOutlineThickness(0);
	m_button.setOutlineColor(outlineColor);
	m_button.setFillColor(fillColor);
	m_button.setSize(size);
	m_button.setPosition(pos);
	
	
	m_text.setCharacterSize(20);
	m_text.setOutlineColor(sf::Color::White);
	m_text.setFillColor(sf::Color::White);
	m_text.setFont(bq::resource_holder::get().fonts.get("arial.ttf"));
	m_text.setCharacterSize(textSize);

	m_text.setPosition(pos);

	m_button.getGlobalBounds().width;
	m_text.move(m_button.getGlobalBounds().width / 4.f,m_button.getGlobalBounds().height / 4.f);
	m_text.setString(label);
}
void bq::gui::Button::setPos(bq::v2f v) {
	m_text.setPosition({0,0});
	m_button.setPosition(v);

	m_text.setPosition({v.x + m_button.getSize().x / 4 , v.y + m_button.getSize().y / 4 });

}