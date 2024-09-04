#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "ResPath/ResPath.hpp"

namespace mk {

	class ResourceManager {
	private:
		ResourceManager() = default;
		sf::Texture&                           getMutTexture(const ResPath& path);
		std::map<ResPath, sf::Texture>         m_textures;
		std::map<sf::Cursor::Type, sf::Cursor> m_system_cursors;
		std::map<ResPath, sf::SoundBuffer>     m_sound_buffers;
		std::map<ResPath, sf::Font>            m_fonts;

	public:
		// singleton stuff
		ResourceManager(const ResourceManager&)                   = delete;
		void                    operator=(const ResourceManager&) = delete;
		static ResourceManager& get();

		const sf::Texture&     getTexture(const ResPath& path);
		const sf::SoundBuffer& getSoundBuffer(const ResPath& path);
		const sf::Cursor&      getSystemCursor(sf::Cursor::Type type);
		sf::Font&              getFont(const ResPath& path);
		void                   setTextureSmooth(const ResPath& path, bool smooth);
	};
}  // namespace mk
