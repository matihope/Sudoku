//
// Created by mateusz on 9/6/23.
//

#pragma once

#include "Math/Math.hpp"
#include "WorldEntity.hpp"

namespace mk {

	class BoxyWorldEntity: public WorldEntity {
	public:
		void setMaxRenderSize(Math::Vector2u newSize);
		void setMaxRenderSize(unsigned int width, unsigned int height);
		[[nodiscard]] Math::Vector2u getMaxSize() const;

		void draw(sf::RenderTarget &target, sf::RenderStates states)
			const override;

	private:
		Math::Vector2u maxSize = { 0, 0 };
	};

}  // namespace mk
