#include "Converters.hpp"
#include <string>

sf::Color mk::Converters::colorFromStrRGB(const std::string& rgb) {
	return sf::Color(std::stoul(rgb, nullptr, 16) * 256 + 0xFF);
}
