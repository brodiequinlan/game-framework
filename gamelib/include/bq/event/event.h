#pragma once

namespace bq {


	enum class event_type {
		KEYPRESSED,KEYRELEASED,MOUSE,VICTORY,CLOSE,NONE
	};

	namespace keyboard {
		enum class keycode {
			W,A,S,D,P,F,UP,DOWN,LEFT,RIGHT,ENTER,SPACE,NONE,TAB
		};
	}
	namespace mouse {
		enum class mousecode {
			LEFT,RIGHT,CENTER,NONE
		};
	}
	class event {
	public:
		event_type type = event_type::NONE;
		bq::keyboard::keycode keycode = keyboard::keycode::NONE;
		bq::mouse::mousecode mousecode = mouse::mousecode::NONE;
		bq::v2f mouse_clicked_pos = { 0,0 };

		event(event_type);
		event(event_type,bq::keyboard::keycode);
		event(event_type, bq::v2f);
		~event();

	};
}