#pragma once

#include "SpriteBatch/SpriteBatch.hpp"
#include "SudokuTileEntity.hpp"

class SudokuBoardEntity: public mk::WorldEntity {
	static constexpr int BOARD_SIZE   = 9;
	static constexpr int SQUARE_COUNT = 3;

	sf::Color even_color{}, odd_color{}, line_color{};

public:
	SudokuBoardEntity();

	void onReady(mk::Game& game) override;

	void load(const sudoku::SudokuBoard& board_data);

private:
	void spawnTiles(mk::Game& game, float tile_size);
	void spawnLines(mk::Game& game, float tile_size);

	mk::SpriteBatch*                                                          tiles = nullptr;
	mk::SpriteBatch*                                                          lines = nullptr;
	std::array<std::array<SudokuTileEntity*, BOARD_SIZE + 1>, BOARD_SIZE + 1> label_tiles{};
};
