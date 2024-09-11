#include "Timer.hpp"
#include "Math/Base.hpp"
#include <algorithm>
#include <cstdint>

namespace {
	std::string intTo2DigitString(uint64_t value) {
		std::string str = "00";
		if (value <= 9)
			str[1] = '0' + value;
		else
			str = std::to_string(value);
		return str;
	}
}

mk::gui::Timer::Timer(sf::Font* font, bool increasing, float target):
	  font(font),
	  increasing(increasing),
	  target(target) {}

std::unique_ptr<mk::gui::Timer> mk::gui::Timer::Increasing(sf::Font* font, float until_seconds) {
	assert(until_seconds > 0.0);
	return std::unique_ptr<mk::gui::Timer>(new Timer(font, true, until_seconds));
}

std::unique_ptr<mk::gui::Timer> mk::gui::Timer::Decreasing(sf::Font* font, float from_seconds) {
	assert(from_seconds > 0.0);
	auto timer          = std::unique_ptr<mk::gui::Timer>(new Timer(font, false, 0));
	timer->current_time = from_seconds;
	return std::move(timer);
}

void mk::gui::Timer::setFont(sf::Font* font) {
	this->font = font;
	label->setFont(font);
}

void mk::gui::Timer::setAlignment(HAlignment newHAlignment, VAlignment newVAlignment) {
	label->setAlignment(newHAlignment, newVAlignment);
}

void mk::gui::Timer::setTextSize(unsigned int newSize) { label->setTextSize(newSize); }

void mk::gui::Timer::setColor(sf::Color newColor) { label->setColor(newColor); }

sf::FloatRect mk::gui::Timer::getBounds() const { return label->getBounds(); }

void mk::gui::Timer::onUpdate(Game& game, float dt) {
	if (stop) return;

	if (!mk::math::isZero(current_time - target)) {
		if (increasing)
			current_time = std::min(current_time + dt, target);
		else
			current_time = std::max(current_time - dt, target);

		uint64_t time_int = current_time;
		uint64_t hours    = time_int / 3'600;
		uint64_t minutes  = (time_int % 3'600) / 60;
		uint64_t seconds  = time_int % 60;
		uint64_t milisecs = uint64_t(current_time * 100) % 100;

		std::string text;

		if (enable_hours) text += intTo2DigitString(hours) + ":";

		text += intTo2DigitString(minutes) + ":" + intTo2DigitString(seconds);

		if (enable_milisecs) text += ":" + intTo2DigitString(milisecs);

		label->setText(text);
	}
}

void mk::gui::Timer::onReady(Game& game) {
	label = addChild<mk::gui::Label>(game, font, "00:00:00");
}

void mk::gui::Timer::enableMilisecs(bool enable) { enable_milisecs = enable; }

void mk::gui::Timer::enableHours(bool enable) { enable_hours = enable; }

void mk::gui::Timer::setStop(bool stop) { this->stop = stop; }

std::string mk::gui::Timer::getText() const { return label->getText(); }
