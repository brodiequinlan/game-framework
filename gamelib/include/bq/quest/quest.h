#pragma once



namespace bq {
	struct quest_event;
	class quest {
	protected:
		bool m_completed = false;
		std::string m_desc;
	public:

		virtual ~quest() = 0;

		bool completed();
		virtual void update() = 0;
		virtual void handleQuestEvent(quest_event&) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
	};
}
