#pragma once

#include "GUI/Label.hpp"
#include "Math/Base.hpp"
#include "WorldEntity/WorldEntity.hpp"
#include <cassert>
#include <memory>

namespace mk::gui {
	class Timer: public WorldEntity {
	public:
		static std::unique_ptr<Timer>
			Increasing(sf::Font* font, float until_seconds = FLOAT_INFINITY);

		static std::unique_ptr<Timer> Decreasing(sf::Font* font, float from_seconds);

		void          setFont(sf::Font* font);
		void          setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);
		void          setTextSize(unsigned int newSize);
		void          setColor(sf::Color newColor);
		sf::FloatRect getBounds() const;
		std::string   getText() const;

		void onUpdate(Game& game, float dt) override;
		void onReady(Game& game) override;

		void enableMilisecs(bool enable);
		void enableHours(bool enable);
		void setStop(bool stop);

	private:
		Timer(sf::Font* font, bool increasing, float target);

		bool  increasing   = false;
		float current_time = 0.0;
		float target       = 0.0;

		bool enable_milisecs = false;
		bool enable_hours    = false;

		sf::Font*       font  = nullptr;
		mk::gui::Label* label = nullptr;

		bool stop = false;
	};
}
