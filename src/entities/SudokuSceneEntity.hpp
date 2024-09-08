#pragma once

#include "GUI/Button.hpp"
#include "SudokuBoardEntity.hpp"
#include "WorldEntity/BoxyWorldEntity.hpp"

class SudokuScene: public mk::WorldEntity {
public:
	SudokuScene(sudoku::SudokuGame::Difficulty difficulty = sudoku::SudokuGame::Difficulty::NORMAL);

	void onReady(mk::Game& game) override;

	void handleEvent(mk::Game& game, const sf::Event& event) override;

	void onUpdate(mk::Game& game, float dt) override {
		if(menu_button->isPressed()) game.popScene();
	}

private:
	void spawnButtons(mk::Game& game);

	SudokuBoardEntity* board = nullptr;
	sudoku::SudokuGame sudoku;

	mk::GUI::Button* menu_button = nullptr;

	std::array<mk::GUI::Button*, 10> number_buttons;
};
