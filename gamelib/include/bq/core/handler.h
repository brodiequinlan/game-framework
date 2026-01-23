#pragma once
#include <bqpch.h>
namespace bq {
	class handler {

		handler() {};
		std::unique_ptr<bq::world> m_world;
		std::unique_ptr<bq::entity_manager> m_em;
		std::unique_ptr<bq::state_manager> m_sm;
		std::unique_ptr<bq::camera> m_cam;
	public:
		
		void set_sm(std::unique_ptr<bq::state_manager> sm);
		void set_em(std::unique_ptr<bq::entity_manager> em);
		void set_cam(std::unique_ptr<bq::camera> cam);
		void set_world(std::unique_ptr<bq::world> world);
		bq::world* world();
		bq::entity_manager* em();
		bq::state_manager* sm();
		bq::camera* cam();
		
		bool has_cam();

		inline static handler& get() {
			static handler h;
			return h;
		}
	};
}
