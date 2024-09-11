#include <GUI/Button.hpp>
#include <Game/Game.hpp>
#include <SFML/Graphics.hpp>
#include <Updatable/Updatable.hpp>

namespace mk::gui {
	Button::Button() {
		m_label.setAlignment(HAlignment::MIDDLE, VAlignment::CENTER);
		m_background.setColor(m_background_color_normal);
	}

	Button::Button(sf::Font* font, const std::string& text): Button() {
		setFont(font);
		setText(text);
	}

	void Button::setAlignment(HAlignment newHAlignment, VAlignment newVAlignment) {
		m_valignment = newVAlignment;
		m_halignment = newHAlignment;
		updateDefaultCollisionShape();
		fixLabelPosition();
	}

	void Button::setFont(sf::Font* newFont) {
		m_label.setFont(newFont);
		updateDefaultCollisionShape();
		fixLabelPosition();
	}

	void Button::setText(const std::string& newText) {
		m_label.setText(newText);
		fixLabelPosition();
		updateDefaultCollisionShape();
	}

	void Button::setTextSize(unsigned int newSize) {
		m_label.setTextSize(newSize);
		fixLabelPosition();
		updateDefaultCollisionShape();
	}

	void Button::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_background, states);
		target.draw(m_label, states);

#if (DEBUG)
		// debugs:
		target.draw(*m_collision_shape, states);
#endif
	}

	void Button::onHover() {
		m_label.setColor(m_font_color_hover);
		m_background.setColor(m_background_color_hover);
	}

	void Button::onStopHover() {
		m_label.setColor(m_font_color_normal);
		m_background.setColor(m_background_color_normal);
	}

	void Button::onHold() {
		m_label.setColor(m_font_color_press);
		m_background.setColor(m_background_color_press);
	}

	void Button::update(Game& game, const float dt) { Clickable::update(game, dt); }

	void Button::updateDefaultCollisionShape() {
		if (m_has_custom_collision_shape) return;

		sf::FloatRect bounds = getBounds();

		m_collision_shape = std::make_unique<RectCollision>(this, bounds.width, bounds.height);
		setClickCollisionShape(m_collision_shape.get());

		m_background.setSize({ bounds.width, bounds.height });

		sf::Vector2f newOrigin{};
		switch (m_valignment) {
		case VAlignment::TOP:
			break;
		case VAlignment::CENTER:
			newOrigin.y = m_background.getSize().y / 2;
			break;
		case VAlignment::BOTTOM:
			newOrigin.y = m_background.getSize().y;
			break;
		}

		switch (m_halignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			newOrigin.x = m_background.getSize().x / 2;
			break;
		case HAlignment::RIGHT:
			newOrigin.x = m_background.getSize().x;
			break;
		}

		m_collision_shape->setPosition(-newOrigin);
		m_background.setOrigin(newOrigin);
	}

	void Button::setCollisionShape(std::unique_ptr<RectCollision> shape) {
		m_has_custom_collision_shape = true;
		setClickCollisionShape(shape.get());
		m_collision_shape = std::move(shape);
	}

	sf::FloatRect Button::getBounds() const {
		// override this when inheriting from Button

		auto bounds = m_label.getBounds();
		bounds.top  = getPosition().y;
		bounds.left = getPosition().x;

		if (minSpaceBetween.x != -1) bounds.width += minSpaceBetween.x * 2;
		if (minSpaceBetween.y != -1) bounds.height += minSpaceBetween.y * 2;

		if (minSize.x != -1) bounds.width = std::max(minSize.x, bounds.width);
		if (minSize.y != -1) bounds.height = std::max(minSize.y, bounds.height);

		switch (m_valignment) {
		case VAlignment::TOP:
			break;
		case VAlignment::CENTER:
			bounds.top -= bounds.height / 2;
			break;
		case VAlignment::BOTTOM:
			bounds.top -= bounds.height;
			break;
		}

		switch (m_halignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			bounds.left -= bounds.width / 2;
			break;
		case HAlignment::RIGHT:
			bounds.left -= bounds.width;
			break;
		}

		return bounds;
	}

	void Button::setBackgroundColors(sf::Color normal, sf::Color hover, sf::Color press) {
		m_background_color_normal = normal;
		m_background_color_hover  = hover;
		m_background_color_press  = press;
	}

	void Button::setBackgroundColors(sf::Color colors) {
		setBackgroundColors(colors, colors, colors);
	}

	void Button::setMinSpaceBetween(math::Vector2f space) {
		minSpaceBetween = space;
		updateDefaultCollisionShape();
		fixLabelPosition();
	}

	void Button::setMinSize(math::Vector2f size) {
		minSize = size;
		updateDefaultCollisionShape();
		fixLabelPosition();
	}

	void Button::fixLabelPosition() {
		auto         bounds = getBounds();
		sf::Vector2f offset = { bounds.width / 2.f, bounds.height / 2.f };
		switch (m_valignment) {
		case VAlignment::TOP:
			break;
		case VAlignment::CENTER:
			offset.y = 0;
			break;
		case VAlignment::BOTTOM:
			offset.y *= -1;
			break;
		}

		switch (m_halignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			offset.x = 0;
			break;
		case HAlignment::RIGHT:
			offset.x *= -1;
			break;
		}

		m_label.setPosition(offset);
	}

	void Button::setFontColors(sf::Color colors) { setFontColors(colors, colors, colors); }

	void Button::setFontColors(sf::Color normal, sf::Color hover, sf::Color press) {
		m_font_color_normal = normal;
		m_font_color_hover  = hover;
		m_font_color_press  = press;
	}
}  // namespace mk::GUI
