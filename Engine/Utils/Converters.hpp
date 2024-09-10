#pragma once

#include "SFML/Graphics/Color.hpp"
#include <string>

namespace mk::converters {
	sf::Color colorFromStrRGB(const std::string& rgb);
	sf::Color colorFromStrRGBA(const std::string& rgb);
}
