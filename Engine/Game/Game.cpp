
#include "Game.hpp"

#include "ResourceManager/ResourceManager.hpp"

#include <CollisionComponent/CollisionComponent.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iterator>

namespace {
	mk::Math::Vector2f
		scaleToFit(const mk::Math::Vector2f viewSize, const mk::Math::Vector2u windowSize) {
		mk::Math::Vector2f scale;
		scale.x = viewSize.x / (float) windowSize.x;
		scale.y = viewSize.y / (float) windowSize.y;
		if (scale.x < scale.y) {
			scale.x *= 1 / scale.y;
			scale.y = 1;
		} else {
			scale.y *= 1 / scale.x;
			scale.x = 1;
		}
		return scale;
	}
}

namespace mk {
	void Game::run() {
		m_run = true;

		if (!m_scenes_stack.empty()) m_scenes_stack.top()->ready(*this);

		while (isRunning()) {
			pollEvents();
			update();
			draw();
		}
	}

	Game::~Game() { m_window.close(); }

	void Game::draw() {
		m_window.clear(sf::Color(21, 21, 21));

		if (!m_scenes_stack.empty()) m_window.draw(*m_scenes_stack.top());

		if (m_enable_print_fps) m_window.draw(m_fps_label);

		m_window.display();
	}

	void Game::update() {
		m_delta_time = m_clock.restart().asSeconds();
		m_mouse_pos  = Math::Vector2f(
            getRenderWindow().mapPixelToCoords(sf::Mouse::getPosition(getRenderWindow()))
        );

		while (!m_safe_scene_delete_queue.empty()) m_safe_scene_delete_queue.pop();

		if (!m_scenes_stack.empty()) {
			m_physics_update_counter += m_delta_time;
			if (m_physics_update_counter >= m_physics_update_call_freq) {
				m_scenes_stack.top()->physicsUpdate(*this, m_physics_update_call_freq);
				m_physics_update_counter -= m_physics_update_call_freq;
			}
			m_scenes_stack.top()->update(*this, m_delta_time);
		}

		recalculateAvgFps();
	}

	void Game::recalculateAvgFps() {
		if (m_enable_print_fps) {
			m_fps_frame_count++;
			m_fps_sum += m_delta_time;
			if (m_fps_sum >= 1.f) {
				int currentFps = (int) round(m_fps_frame_count / m_fps_sum);
				m_fps_label.setText(std::to_string(currentFps));
				m_fps_sum         = 0.0;
				m_fps_frame_count = 0;
			}
		}
	}

	void Game::addScene(std::unique_ptr<WorldEntity> newScene) {
		if (isRunning()) newScene->ready(*this);
		m_scenes_stack.push(std::move(newScene));
	}

	void Game::popScene() {
		m_safe_scene_delete_queue.push(std::move(m_scenes_stack.top()));
		m_scenes_stack.pop();
	}

	void Game::replaceTopScene(std::unique_ptr<WorldEntity> newScene) {
		popScene();
		addScene(std::move(newScene));
	}

	void Game::pollEvents() {
		sf::Event event{};
		while (m_window.pollEvent(event)) {
			if (!m_scenes_stack.empty()) m_scenes_stack.top()->handleEvent(*this, event);

			switch (event.type) {
			case sf::Event::Closed:
				stop();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Tilde:
					popScene();
					if (m_scenes_stack.empty()) stop();
					break;
				default:
					// skip any other case
					break;
				}
				break;
			case sf::Event::Resized:
				updateViewportSize();
				break;
			default:
				// skip any other case
				break;
			}
		}
	}

	bool Game::isRunning() const { return m_run; }

	void Game::stop() { m_run = false; }

	void Game::setPrintFPS(const bool& printFPS) { m_enable_print_fps = printFPS; }

	Math::Vector2u Game::getWindowSize() { return Math::Vector2u(m_window.getSize()); }

	Math::Vector2u Game::getViewportSize() { return Math::Vector2u(m_view.getSize()); }

	sf::RenderWindow& Game::getRenderWindow() { return m_window; }

	void Game::updateViewportSize() {
		Math::Vector2f viewportScale
			= scaleToFit(Math::Vector2f(m_view.getSize()), getWindowSize());
		m_view.setViewport(sf::FloatRect(
			sf::Vector2f(0.5f - viewportScale.x / 2, 0.5f - viewportScale.y / 2),
			viewportScale.as<sf::Vector2f>()
		));
		m_window.setView(m_view);
	}

	Math::Vector2f Game::getMousePos() { return m_mouse_pos; }

	const sf::View* Game::getView() { return &m_view; }

	void Game::setCameraCenterAt(const sf::Vector2f& pos) {
		m_view.setCenter(pos);
		m_window.setView(m_view);
		// polsrodek
		m_fps_label.setPosition(
			pos - sf::Vector2f(384. / 2, 216. / 2)
			+ sf::Vector2f(
				float(m_game_settings["viewport"]["width"]) - 1,
				1
			)
		);  // a
	}

	void Game::setCursor(sf::Cursor::Type type) {
		if (m_current_cursor_type == type) return;
		m_current_cursor_type = type;

		getRenderWindow().setMouseCursor(ResourceManager::get().getSystemCursor(type));
	}

	bool Game::isWindowActive() const { return m_window.hasFocus(); }

	void Game::setViewportSize(sf::Vector2f newSize) {
		m_view.reset(sf::FloatRect(0, 0, newSize.x, newSize.y));
		updateViewportSize();
		m_fps_label.setPosition(newSize.x - 1, 1);
	}

	void Game::setViewportSize(sf::Vector2u newSize) {
		setViewportSize(sf::Vector2f(float(newSize.x), float(newSize.y)));
	}

	Game::Game(const ResPath& settingsPath) {
		m_game_settings.load(settingsPath);
		m_window.create(
			sf::VideoMode(m_game_settings["window"]["width"], m_game_settings["window"]["height"]),
			std::string(m_game_settings["window"]["title"]),
			sf::Style::Default
		);
		m_window.setVerticalSyncEnabled(m_game_settings["window"]["vsync"]);
		setViewportSize(sf::Vector2u(
			m_game_settings["viewport"]["width"], m_game_settings["viewport"]["height"]
		));

		if (!icon.loadFromFile(ResPath("icon.png").strPath()))
			throw std::runtime_error("Cannot find icon.png in assets");

		m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		setPrintFPS(m_game_settings["debug"]["printFPS"]);
		m_physics_update_call_freq
			= 1.f / float(m_game_settings["engine"]["physicsUpdateCallFreq"]);
		Debug::setDebugCollisionDraw(m_game_settings["debug"]["drawCollisionShapes"]);

		m_default_font
			= &ResourceManager::get().getFont(ResPath(m_game_settings["engine"]["fontPath"]));

		m_fps_label.setFont(m_default_font);
		m_fps_label.setText("0");
		m_fps_label.setTextSize(32);
		m_fps_label.setAlignment(GUI::HAlignment::RIGHT, GUI::VAlignment::TOP);

		// lastly, set cursor
		m_current_cursor_type = sf::Cursor::Arrow;
		setCursor(sf::Cursor::Arrow);
	}

	sf::Font* Game::getDefaultFont() const { return m_default_font; }

}  // namespace mk
