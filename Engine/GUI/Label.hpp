#pragma once
#include <SFML/Graphics.hpp>
#include <WorldEntity/WorldEntity.hpp>

namespace mk::gui {
	enum class HAlignment { LEFT, MIDDLE, RIGHT };

	enum class VAlignment { TOP, CENTER, BOTTOM };

	class Label: public WorldEntity {
		sf::Font*   m_font{};
		HAlignment  m_halignment = HAlignment::LEFT;
		VAlignment  m_valignment = VAlignment::TOP;
		sf::Text    m_text;
		std::string m_string;

	public:
		Label();
		explicit Label(sf::Font* font);

		Label(sf::Font* font, const std::string& text);

		void          setFont(sf::Font* font);
		void          setText(const std::string& text);
		void          setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);
		void          setTextSize(unsigned int newSize);
		void          setColor(sf::Color newColor);
		void          onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
		sf::FloatRect getBounds() const;

		const std::string& getText() const;
	};
}  // namespace mk::gui
