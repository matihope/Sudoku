//
// Created by mateusz on 8/31/23.
//

#pragma once

#include "WorldEntity/WorldEntity.hpp"

namespace mk {
	class RayShape: public WorldEntity {
	public:
		RayShape() = default;
		RayShape(sf::Color color, float width, float length, float wingLength);
		void onDraw(sf::RenderTarget &target, sf::RenderStates states)
			const override;
		void setSize(float width, float length, float wingLength);
		void setColor(sf::Color color);
		void pointAt(sf::Vector2f arrowHead);

	private:
		float              width{}, length{}, wingLength{};
		sf::RectangleShape body;
		sf::RectangleShape leftWing, rightWing;
		sf::CircleShape    arrowFill;
	};
}  // namespace mk
