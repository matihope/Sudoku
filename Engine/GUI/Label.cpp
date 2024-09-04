#include "Game/Game.hpp"

#include <GUI/Label.hpp>
#include <iostream>

namespace mk::GUI {
	Label::Label() {
		m_text.setCharacterSize(32);
	}

	Label::Label(sf::Font *font): Label() { setFont(font); }

	void Label::setFont(sf::Font *font) {
		m_text.setFont(*font);
		m_font = font;
		setTextSize(m_text.getCharacterSize());
	}

	void Label::setText(const std::string &newText) {
		m_text.setString(newText);
		m_string = newText;
		setAlignment(m_halignment, m_valignment);
	}

	void Label::setAlignment(
		HAlignment newHAlignment, VAlignment newVAlignment
	) {
		m_halignment   = newHAlignment;
		m_valignment   = newVAlignment;
		int textWidth  = (int) m_text.getGlobalBounds().width;
		int textHeight = (int) m_text.getGlobalBounds().height;

		int newlines = (int) std::ranges::count(m_string, '\n');
		//		textHeight *= newlines + 1;

		sf::Vector2f newPos(
			-m_text.getLocalBounds().left, -m_text.getLocalBounds().top
		);
		switch (m_halignment) {
		case HAlignment::LEFT:
			break;
		case HAlignment::MIDDLE:
			newPos.x -= (float) textWidth / 2;
			break;
		case HAlignment::RIGHT:
			newPos.x -= (float) textWidth;
			break;
		}
		switch (m_valignment) {
		case VAlignment::TOP:
			//			newPos.y += (float) textHeight;
			break;
		case VAlignment::CENTER:
			newPos.y -= (float) textHeight / 2;
			break;
		case VAlignment::BOTTOM:
			newPos.y -= (float) textHeight;
			break;
		}
		m_text.setPosition((int) newPos.x, (int) newPos.y);
	}

	void
		Label::onDraw(sf::RenderTarget &target, sf::RenderStates states) const {
		if (m_text.getString() == "") return;
		states.transform *= getTransform();
		target.draw(m_text, states);
	}

	void Label::setTextSize(const unsigned int newSize) {
		m_text.setCharacterSize(newSize);
		setAlignment(m_halignment, m_valignment);
	}

	void Label::setColor(const sf::Color newColor) {
		m_text.setFillColor(newColor);
	}

	sf::FloatRect Label::getBounds() const {
		auto bounds = m_text.getGlobalBounds();
		bounds.left += getPosition().x;
		bounds.top += getPosition().y;
		return bounds;
	}

	const std::string &Label::getText() const { return m_string; }

}  // namespace mk::GUI
