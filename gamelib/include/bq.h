#pragma once


#include <bq/util/nonmoveable.h>
#include <bq/util/noncopyable.h>
//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>

//stl includes
#include <stack>
#include <map>
#include <iostream>
#include <functional>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <optional>
#include <exception>
#include <algorithm>
#include <chrono>
#include <bq/util/vec.h>
#include <bq/core/astar.h>
#include <bq/application/application.h>
#include <bq/util/block_collision_effects.h>
#include <bq/entity/entity.h>
#include <bq/entity/entity_manager.h>
#include <bq/core/buff.h>
#include <bq/graphics/window.h>
#include <bq/core/game.h>
#include <bq/util/random.h>
#include <bq/state/state.h>
#include <bq/state/state_manager.h>
#include <bq/core/item.h>
#include <bq/resource/resource_holder.h>
#include <bq/core/camera.h>
#include <bq/core/inventory.h>
#include <bq/world/world.h>
#include <bq/world/block.h>
#include <bq/gui/button.h>
#include <bq/gui/health_bar.h>
#include <bq/graphics/animation.h>
#include <bq/quest/quest_event.h>
#include <bq/quest/quest.h>
#include <bq/entity/particle.h>
#include <bq/event/event.h>
#include <bq/graphics/sprite.h>
#include <bq/core/profiler.h>
#include <bq/networking/network_result.h>
#include <bq/networking/http_client.h>
#include <bq/core/handler.h>
#include <bq/exception/illegal_state.h>
#include <bq/world/block.h>
constexpr int GAME_HEIGHT = INTL_HEIGHT;
constexpr int GAME_WIDTH = INTL_WIDTH;

#ifdef DEBUG
#define BQ_PROFILE(x) auto profiler = bq::profiler(x);
#else
#define BQ_PROFILE(x)
#endif