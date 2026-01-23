#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <cpp-httplib/httplib.h>
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
#include <chrono>
//bq include 
//note if adding bq include: if you add a file that changes often here, you're actually harming compile performance here.
#include <bq/core/logger.h>
#include <bq/util/vec.h>
#include <bq/util/random.h>
#include <bq/util/nonmoveable.h>
#include <bq/util/noncopyable.h>
#include <bq/util/json.h>


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

#define INTL_HEIGHT 512
#define INTL_WIDTH 900