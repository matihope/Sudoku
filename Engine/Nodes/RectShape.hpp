//
// Created by mateusz on 8/30/23.
//

#pragma once

#include "WorldEntity/WorldEntity.hpp"

namespace mk {

	class RectShape: public WorldEntity {
	public:
		RectShape() = default;
		RectShape(sf::Color myColor, mk::math::Vector2f size);
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setColor(const sf::Color& color);

	private:
		sf::RectangleShape rect;
	};

}  // namespace mk
