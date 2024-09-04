//
// Created by mateusz on 3/14/23.
//

#pragma once
#include <SFML/Graphics.hpp>

namespace mk {

	class QuickSprite {
	public:
		explicit QuickSprite(sf::Vertex *quad);

		void                       setSize(sf::Vector2f newSize);
		[[nodiscard]] sf::Vector2f getSize() const;

		void                       setPosition(sf::Vector2f position);
		[[nodiscard]] sf::Vector2f getPosition() const;

		void                       setTexSize(sf::Vector2f newSize);
		[[nodiscard]] sf::Vector2f getTexSize() const;

		void                       setTexPosition(sf::Vector2f position);
		[[nodiscard]] sf::Vector2f getTexPosition() const;

		void setColor(
			sf::Color vert0, sf::Color vert1, sf::Color vert2, sf::Color vert3
		);
		void                    setColor(sf::Color color);
		[[nodiscard]] sf::Color getColor(unsigned int vertexId);

		// top left pixel should be transparent!!!
		void clear();

	private:
		float       M_TEX_BUFFER = 0.04;
		sf::Vertex *m_quad;
	};
}  // namespace mk
