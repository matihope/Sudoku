//
// Created by mateusz on 8/30/23.
//

#pragma once

#include "WorldEntity/WorldEntity.hpp"

namespace mk {
	class CircleShape: public WorldEntity {
	public:
		CircleShape() = default;
		CircleShape(sf::Color myColor, float radius);
		void onDraw(sf::RenderTarget &target, sf::RenderStates states)
			const override;
		void                setRadius(float radius);
		[[nodiscard]] float getRadius() const;

	private:
		sf::CircleShape circle;
	};
}  // namespace mk
