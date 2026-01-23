#include <bqpch.h>
#include <bq/event/event.h>
#include <bq/state/state.h>
#include <bq/entity/entity.h>
#include <bq/entity/entity_manager.h>
#include <bq/state/state_manager.h>
#include <bq/exception/illegal_state.h>
#include <bq/world/world.h>
#include <bq/core/camera.h>
#include <bq/core/handler.h>
void bq::handler::set_sm(std::unique_ptr<bq::state_manager> sm) {
    m_sm = std::move(sm);
}

void bq::handler::set_em(std::unique_ptr<bq::entity_manager> em) {
    m_em = std::move(em);
}

void bq::handler::set_cam(std::unique_ptr<bq::camera> cam) {
    m_cam = std::move(cam);
}

void bq::handler::set_world(std::unique_ptr<bq::world> world) {
    m_world = std::move(world);
}

bq::world* bq::handler::world() {
#ifdef DEBUG
    if (!m_world) {
        bq::logger::warn("m_world is null on access");
        throw bq::illegal_state("world null on access");
    }
#endif
    return m_world.get();
}
bq::entity_manager* bq::handler::em() {
#ifdef DEBUG
    if (!m_em) {
        bq::logger::warn("m_em is null on access");
        throw bq::illegal_state("entity_manager null on access");
    }
#endif
    return m_em.get();
}
bq::state_manager* bq::handler::sm() {
#ifdef DEBUG
    if (!m_sm) {
        bq::logger::warn("m_sm is null on access");
        throw bq::illegal_state("state_manager null on access");
    }
#endif
    return m_sm.get();
}
bool bq::handler::has_cam() {
    return !!m_cam;
}
bq::camera* bq::handler::cam() {
#ifdef DEBUG
    if (!m_cam) {
        bq::logger::warn("m_cam is null on access");
        throw bq::illegal_state("camera null on access");
    }
#endif
    return m_cam.get();
}