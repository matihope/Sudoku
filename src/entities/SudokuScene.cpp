#include "SudokuScene.hpp"

SudokuScene::SudokuScene(sudoku::SudokuGame::Difficulty difficulty): sudoku(difficulty) {}

void SudokuScene::onReady(mk::Game& game) {
	board = addChild<SudokuBoardEntity>(game);
	board->load(sudoku.getBoard());
}

void SudokuScene::handleEvent(mk::Game& game, const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
		game.replaceTopScene(std::make_unique<SudokuScene>());
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		sudoku.fill();
		board->load(sudoku.getBoard());
	}
}
