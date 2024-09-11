#pragma once
#include <CollisionComponent/CollisionComponent.hpp>
#include <Updatable/Updatable.hpp>
#include <memory>

namespace mk {
	class Clickable: public Updatable {
	public:
		enum class ClickMode { PressOnRelease, PressOnClick };

		Clickable();
		virtual ~Clickable() = 0;
		[[nodiscard]]
		bool isPressed() const;
		void update(Game& game, float dt) override;
		void setClickCollisionShape(CollisionComponent* collision_shape);
		void setClickMode(ClickMode new_mode);

	protected:
		virtual void onHover() {}  // hovering mouse over m_collision_shape

		virtual void onStopHover() {}

		virtual void onHold() {}     // function called during holding

		virtual void onRelease() {}  // mouse is released over the m_collision_shape

		virtual void onPressed() {}  // callback

	private:
		ClickMode           m_click_mode = ClickMode::PressOnRelease;
		CollisionComponent* m_collision_shape;
		bool                m_is_held;
		bool                m_is_pressable;
		bool                m_was_held_prev;
		bool                m_is_pressed;
		void                makePress();
	};
}  // namespace mk
