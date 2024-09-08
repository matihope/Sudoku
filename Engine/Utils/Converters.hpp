#pragma once

#include "SFML/Graphics/Color.hpp"
#include <string>

namespace mk::Converters {
	sf::Color colorFromStrRGB(const std::string& rgb);
}
