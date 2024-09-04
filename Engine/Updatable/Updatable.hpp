#pragma once
#include <memory>

namespace mk {
    class Game;
	class Updatable {
	public:
		virtual void update(Game& game, const float dt){};
		virtual void physicsUpdate(Game& game, const float dt){};
		virtual void ready(Game& game){};
	};
}  // namespace mk
