#pragma once

#include "SudokuBoardEntity.hpp"

class SudokuScene: public mk::WorldEntity {
public:
	SudokuScene(sudoku::SudokuGame::Difficulty difficulty = sudoku::SudokuGame::Difficulty::NORMAL);

	void onReady(mk::Game& game) override;

	void handleEvent(mk::Game& game, const sf::Event& event) override;

private:
	void spawnButtons() {}

	SudokuBoardEntity* board = nullptr;
	sudoku::SudokuGame sudoku;

	//	mk::GUI::Button*
};
