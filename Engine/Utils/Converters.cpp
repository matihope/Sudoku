#include "Converters.hpp"
#include <string>

sf::Color mk::converters::colorFromStrRGB(const std::string& rgb) {
	return sf::Color(std::stoul(rgb, nullptr, 16) * 256 + 0xFF);
}

sf::Color mk::converters::colorFromStrRGBA(const std::string& rgb) {
	return sf::Color(std::stoul(rgb, nullptr, 16));
}
