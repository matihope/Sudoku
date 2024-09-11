#pragma once

#include "GUI/Button.hpp"
#include "GUI/Timer.hpp"
#include "SudokuBoardEntity.hpp"
#include "SudokuGame.hpp"

class SudokuScene: public mk::WorldEntity {
public:
	SudokuScene(sudoku::SudokuGame::Difficulty difficulty = sudoku::SudokuGame::Difficulty::NORMAL);

	void onReady(mk::Game& game) override;

	void handleEvent(mk::Game& game, const sf::Event& event) override;

	void onUpdate(mk::Game& game, float dt) override;

private:
	void spawnButtons(mk::Game& game);

	void handlePutDigit(sudoku::SudokuValue digit);

	void undo();

	SudokuBoardEntity*             board        = nullptr;
	bool                           taking_notes = false;
	sudoku::SudokuGame             sudoku;
	sudoku::SudokuGame::Difficulty difficulty;

	mk::gui::Button* menu_button = nullptr;
	mk::gui::Button* note_button = nullptr;
	mk::gui::Button* undo_button = nullptr;

	std::array<mk::gui::Button*, 10> number_buttons{};

	mk::gui::Timer* timer = nullptr;
};
