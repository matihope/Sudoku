#pragma once

#include "GUI/Label.hpp"
#include "JsonBridge/JsonBridge.hpp"
#include "Updatable/Updatable.hpp"
#include "WorldEntity/WorldEntity.hpp"
#include "Math/Vector2.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <stack>

namespace mk {

	class Game {
		bool             m_run                      = false;
		float            m_delta_time               = 0;
		double           m_fps_sum                  = 0.0;
		unsigned int     m_fps_frame_count          = 0;
		bool             m_enable_print_fps         = false;
		float            m_physics_update_call_freq = 1. / 60.;
		float            m_physics_update_counter   = 0.f;
		sf::Cursor::Type m_current_cursor_type;

		sf::Clock                                m_clock;
		JsonBridge                               m_game_settings;
		sf::RenderWindow                         m_window;
		std::stack<std::unique_ptr<WorldEntity>> m_scene_stack;
		sf::View                                 m_view;
		sf::Font*                                m_default_font;
		GUI::Label                               m_fps_label;
		Math::Vector2f                           m_mouse_pos;
		sf::Image                                icon;

		void                                     updateViewportSize();
		void                                     recalculateAvgFps();
		std::queue<std::unique_ptr<WorldEntity>> m_safe_scene_delete_queue;

	public:
		explicit Game(const ResPath& settings);
		~Game();

		void run();
		void draw();
		void update();
		void pollEvents();
		void stop();

		void              setPrintFPS(const bool& printFPS);
		bool              isRunning() const;
		Math::Vector2u    getWindowSize();
		Math::Vector2u    getViewportSize();
		void              setViewportSize(sf::Vector2f newSize);
		void              setViewportSize(sf::Vector2u newSize);
		sf::RenderWindow& getRenderWindow();
		bool              isWindowActive() const;

		void addScene(std::unique_ptr<WorldEntity> newScene);
		void replaceTopScene(std::unique_ptr<WorldEntity> newScene);

		/**
		 * @brief Immediately replace a top scene, but schedule the old one for safe deletion.
		 */
		void popScene();

		Math::Vector2f  getMousePos();
		const sf::View* getView();
		void            setCameraCenterAt(const sf::Vector2f& pos);
		void            setCursor(sf::Cursor::Type type);
		sf::Font*       getDefaultFont() const;
	};

}  // namespace mk
