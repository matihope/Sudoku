//
// Created by mateusz on 8/30/23.
//

#include "RectShape.hpp"

namespace mk {
	RectShape::RectShape(sf::Color myColor, mk::math::Vector2f size) {
		rect.setFillColor(myColor);
		rect.setSize(size.as<sf::Vector2f>());
	}

	void RectShape::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(rect, states);
	}

	void RectShape::setColor(const sf::Color& color) { rect.setFillColor(color); }

	void RectShape::setSize(const mk::math::Vector2f& size) {
		rect.setSize(size.as<sf::Vector2f>());
	}

	mk::math::Vector2f RectShape::getSize() const { return { rect.getSize().x, rect.getSize().y }; }
}  // namespace mk
