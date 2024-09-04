#include <ResourceManager/ResourceManager.hpp>

namespace mk {
	ResourceManager& ResourceManager::get() {
		static ResourceManager instance;
		return instance;
	}

	sf::Texture& ResourceManager::getMutTexture(const ResPath& path) {
		if (!m_textures.contains(path)) m_textures[path].loadFromFile(path.getPath());
		return m_textures[path];
	}

	const sf::Texture& ResourceManager::getTexture(const ResPath& path) {
		return getMutTexture(path);
	}

	void ResourceManager::setTextureSmooth(const ResPath& path, bool smooth) {
		getMutTexture(path).setSmooth(smooth);
	}

	const sf::Cursor& ResourceManager::getSystemCursor(sf::Cursor::Type type) {
		if (!m_system_cursors.contains(type)) m_system_cursors[type].loadFromSystem(type);
		return m_system_cursors[type];
	}

	const sf::SoundBuffer& ResourceManager::getSoundBuffer(const ResPath& path) {
		if (!m_sound_buffers.contains(path)) m_sound_buffers[path].loadFromFile(path.getPath());
		return m_sound_buffers[path];
	}

	sf::Font& ResourceManager::getFont(const ResPath& path) {
		if (!m_fonts.contains(path)) m_fonts[path].loadFromFile(path.getPath());
		return m_fonts[path];
	}
}  // namespace mk
