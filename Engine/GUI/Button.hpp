#pragma once
#include "Math/Vector2.hpp"
#include "Nodes/RectShape.hpp"

#include <Clickable/Clickable.hpp>
#include <CollisionShape/CollisionShape.hpp>
#include <GUI/Label.hpp>
#include <SFML/Graphics.hpp>
#include <Updatable/Updatable.hpp>
#include <memory>

namespace mk::gui {
	class Button: public WorldEntity, public Clickable {
		Button();

		Label              m_label;
		sf::RectangleShape m_background;

		sf::Color m_background_color_normal = sf::Color(60, 60, 60);
		sf::Color m_background_color_hover  = sf::Color(50, 50, 50);
		sf::Color m_background_color_press  = sf::Color(40, 40, 40);

		sf::Color m_font_color_normal = sf::Color(255, 255, 255);
		sf::Color m_font_color_hover  = sf::Color(200, 200, 200);
		sf::Color m_font_color_press  = sf::Color(125, 125, 125);

		std::unique_ptr<RectCollision> m_collision_shape;
		bool                           m_has_custom_collision_shape = false;

		HAlignment m_halignment = HAlignment::LEFT;
		VAlignment m_valignment = VAlignment::TOP;

		// minimal size of the button background
		math::Vector2f minSize = { -1, -1 };

		// space between font border end and an edge
		math::Vector2f minSpaceBetween = { -1, -1 };

		void fixLabelPosition();

	protected:
		void onStopHover() override;
		void onHover() override;
		void onHold() override;

	public:
		Button(sf::Font* font, const std::string& text);

		void setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);

		void setFont(sf::Font* font);
		void setText(const std::string& newText);
		void setTextSize(unsigned int newSize);

		void update(Game& game, float dt) override;
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void          updateDefaultCollisionShape();
		void          setCollisionShape(std::unique_ptr<RectCollision> shape);
		sf::FloatRect getBounds() const;

		void setBackgroundColors(sf::Color normal, sf::Color hover, sf::Color press);
		void setBackgroundColors(sf::Color colors);
		void setFontColors(sf::Color normal, sf::Color hover, sf::Color press);
		void setFontColors(sf::Color colors);

		void setMinSpaceBetween(math::Vector2f space);
		void setMinSize(math::Vector2f size);
	};
}  // namespace mk::GUI
