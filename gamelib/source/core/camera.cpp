#include "bqpch.h"
#include <bq/entity/entity.h>
#include <bq/core/camera.h>
#include <bq/core/handler.h>
#include <bq/world/world.h>
#include <bq/state/state.h>
void bq::camera::render(sf::RenderWindow& window) const {
	window.setView(m_view);
}

void bq::camera::update() {
	m_view.setCenter( m_target->pos() );
}

bq::camera::camera(bq::entity* e): m_target(e) {
	m_view.setSize(1920, 1080);
}
const sf::View& bq::camera::view() const
{
	return m_view;
}
void bq::camera::reset() {
	m_view.setCenter({ 1920 / 2,1080 / 2 });
}
