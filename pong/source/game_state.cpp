#include "game_state.h"
#include "ball.h"
#include "paddle.h"
game_state::game_state() {

	bq::handler::get().set_em(std::make_unique<bq::entity_manager>());
	bq::handler::get().em()->add(std::make_unique<ball>());
	bq::handler::get().em()->add(std::make_unique<paddle>(bq::v2f(0,GAME_HEIGHT/2), sf::Keyboard::W, sf::Keyboard::S));
	bq::handler::get().em()->add(std::make_unique<paddle>(bq::v2f(1910, GAME_HEIGHT/2), sf::Keyboard::Up, sf::Keyboard::Down));
}

void game_state::render(sf::RenderWindow& window) {
	bq::handler::get().em()->render(window);
}

void game_state::update() {
	bq::handler::get().em()->update();
}

void game_state::handleEvents(sf::Event& e, sf::RenderWindow& window) {
	bq::handler::get().em()->handleEvent(e);
}