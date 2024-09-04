//
// Created by mateusz on 8/30/23.
//

#include "RectShape.hpp"

namespace mk {
	RectShape::RectShape(sf::Color myColor, sf::Vector2f size) {
		rect.setFillColor(myColor);
		rect.setSize(size);
	}

	void RectShape::onDraw(sf::RenderTarget &target, sf::RenderStates states)
		const {
		states.transform *= getTransform();
		target.draw(rect, states);
	}
}  // namespace mk
