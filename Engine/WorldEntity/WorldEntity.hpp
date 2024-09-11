#pragma once
#include <CollisionComponent/CollisionComponent.hpp>
#include <SFML/Graphics.hpp>
#include <Updatable/Updatable.hpp>
#include <WorldEntity/WorldEntity.hpp>
#include <list>
#include <map>
#include <memory>

namespace mk {
	typedef unsigned long long EntityID;

	class WorldEntity: public sf::Drawable, public sf::Transformable, public Updatable {
	private:
		static EntityID id_counter;
		EntityID        m_entityId;

		bool         m_toKill = false;
		WorldEntity* m_parent;

		bool m_show;

		void cleanEntities();

		bool m_called_ready = false;

	public:
		friend class BoxyWorldEntity;

		WorldEntity();
		~WorldEntity() override = default;

		EntityID getId() const;

		void        queueFree();
		const bool& isDying() const;

		std::map<unsigned int, std::list<std::unique_ptr<WorldEntity>>> m_entity_pool;

		void         addParent(WorldEntity* parent);
		WorldEntity* getParent();

		template<class T, unsigned int drawOrder = 0>
		requires std::is_base_of_v<WorldEntity, T>
		T* addChild(Game& game, std::unique_ptr<T> child) {
			child->addParent(this);
			auto my_child = child.get();
			m_entity_pool[drawOrder].push_back(std::move(child));
			if (m_called_ready) m_entity_pool[drawOrder].back()->ready(game);
			return my_child;
		}

		template<class T, unsigned int drawOrder = 0, class... Args>
		requires std::is_base_of_v<WorldEntity, T> T* addChild(Game& game, Args&&... args) {
			auto new_child     = std::make_unique<T>(std::forward<Args>(args)...);
			auto new_child_ptr = new_child.get();
			return addChild<T, drawOrder>(game, std::move(new_child));
		}

		void ready(Game& game) override;
		void update(Game& game, float dt) override;
		void physicsUpdate(Game& game, float dt) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		virtual void onReady(Game& game) {}

		virtual void onUpdate(Game& game, float dt) {}

		virtual void onPhysicsUpdate(Game& game, float dt) {}

		virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const {}

		virtual void handleEvent(Game& game, const sf::Event& event) {}

		void show();
		void hide();

		sf::Vector2f  getGlobalPosition() const;
		sf::Transform getGlobalTransform() const;
	};
}  // namespace mk
