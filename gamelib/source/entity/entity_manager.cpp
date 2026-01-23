#include "bqpch.h"

#include <bq/entity/entity.h>
#include <bq/core/camera.h>
#include <bq/quest/quest_event.h>
#include <bq/quest/quest.h>
#include <bq/world/world.h>
#include <bq/entity/entity_manager.h>
#include <bq/state/state.h>
#include <bq/state/state_manager.h>
#include <bq/core/handler.h>
#include <bq/state/state.h>
#include <bq/event/event.h>
#include <bq/graphics/window.h>
bq::entity_manager::entity_manager() {}

void bq::entity_manager::add(std::unique_ptr<bq::entity> e) {
	m_to_add.push_back(std::move(e));
}
void bq::entity_manager::remove(entity* e) {
	m_to_remove.push_back(e);
}
void bq::entity_manager::remove_marked() {
	for (auto& e : m_to_remove) {
		for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
			if ((e) == (*it).get()) {

				if (m_quest != nullptr && (*it)->id() != 9) {

					quest_event qe(bq::quest_tpye::COMBAT, 1, (*it)->id());
					m_quest->handleQuestEvent(qe);
				}
				m_entities.erase(it);
				break;
			}
		}
	}
	m_to_remove.clear();
}
void bq::entity_manager::add_marked() {
	for (auto& e : m_to_add) {
		m_entities.push_back(std::move(e));
	}
	m_to_add.clear();
}
void bq::entity_manager::update() {

	for (std::unique_ptr<entity>& e : m_entities) {
			if (e->should_cull(bq::handler::get().cam()->view())) {
				m_to_remove.push_back(e.get());
			}
			else e->update();
		
	}
	remove_marked();
	add_marked();
#ifdef _DEBUG
	if (m_entities.size() == 0) {
		bq::logger::warn("Entitites vector is empty - entity_manager::update()");
	}
	else {
		if (static_cast<int>(bq::random::getRandom(1, 100)) == 2) bq::logger::debug("Entities: " + std::to_string(m_entities.size()));
	}
#endif
}
void bq::entity_manager::render(bq::window& window) {
	for (auto& e : m_entities) {
		e->render(window);
	}
}
void bq::entity_manager::handleEvent(bq::event& evt) {
	for (auto& e : m_entities) {
		e->handle_event(evt);
	}
}

size_t bq::entity_manager::register_id(const std::string& name)
{
	if (m_id_map.find(name) != m_id_map.end()) {
		return m_id_map[name];
	}
	m_id_map[name] = ++m_current_id;
	return m_current_id;
}

size_t bq::entity_manager::get_id(const std::string& name)
{
	try {
		return m_id_map[name];
	}
	catch (...) {
		bq::logger::warn("name is not registered in id system: " + name);
		return -1;
	}
}

const std::vector<std::unique_ptr<bq::entity>>& bq::entity_manager::entities() const
{
	return m_entities;
}

void bq::entity_manager::hook_quest(bq::quest* q)
{
	m_quest = q;
}

void bq::entity_manager::unhook_quest(bq::quest* q)
{
	if (m_quest == q) {
		//we do now own m_quest
		m_quest = nullptr;
	}
}

bq::entity* bq::entity_manager::intersects(sf::FloatRect& other, size_t requester_id, bool find_same_type)
{
	for (auto& e : m_entities) {
		if (e->intersects(other)) {
			if (requester_id == e->id() && !find_same_type) {
				continue;
			}
			return e.get();
		}
	}
	return nullptr;
}
