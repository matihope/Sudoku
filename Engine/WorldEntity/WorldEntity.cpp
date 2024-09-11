#include "WorldEntity.hpp"

#include <WorldEntity/WorldEntity.hpp>

namespace mk {

	EntityID WorldEntity::id_counter = 0;

	WorldEntity::WorldEntity(): m_entityId(id_counter++) {
		m_parent = nullptr;
		m_show   = true;
	}

	EntityID WorldEntity::getId() const { return m_entityId; }

	void WorldEntity::queueFree() { m_toKill = true; }

	const bool& WorldEntity::isDying() const { return m_toKill; }

	void WorldEntity::cleanEntities() {
		for (auto& layer: m_entity_pool) {
			for (auto it = layer.second.begin(); it != layer.second.end(); it++) {
				WorldEntity* entity = it->get();
				if (entity->isDying()) it = layer.second.erase(it);
			}
		}
	}

	void WorldEntity::physicsUpdate(Game& game, const float dt) {
		onPhysicsUpdate(game, dt);
		for (const auto& layer: m_entity_pool)
			for (auto& entity: layer.second) entity->physicsUpdate(game, dt);
	}

	void WorldEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (m_show) {
			sf::RenderStates copied_states(states);
			copied_states.transform *= getTransform();
			onDraw(target, states);

			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) target.draw(*entity, copied_states);
		}
	}

	void WorldEntity::addParent(WorldEntity* parent) { m_parent = parent; }

	WorldEntity* WorldEntity::getParent() { return m_parent; }

	void WorldEntity::update(Game& game, float dt) {
		cleanEntities();
		onUpdate(game, dt);
		for (const auto& layer: m_entity_pool)
			for (auto& entity: layer.second) entity->update(game, dt);
	}

	void WorldEntity::show() { m_show = true; }

	void WorldEntity::hide() { m_show = false; }

	sf::Transform WorldEntity::getGlobalTransform() const {
		if (m_parent == nullptr) return getTransform();
		return m_parent->getGlobalTransform() * getTransform();
	}

	sf::Vector2f WorldEntity::getGlobalPosition() const {
		if (m_parent == nullptr) return getPosition();
		return getPosition() + m_parent->getGlobalPosition();
	}

	void WorldEntity::ready(Game& game) {
		if (!m_called_ready) {
			m_called_ready = true;
			onReady(game);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->ready(game);
		}
	}
}  // namespace mk
