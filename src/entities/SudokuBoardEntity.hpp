#pragma once

#include "SpriteBatch/SpriteBatch.hpp"
#include "SudokuGame.hpp"
#include "SudokuTileEntity.hpp"
#include "Utils/Rect.hpp"

class SudokuBoardEntity: public mk::WorldEntity {
	static constexpr int BOARD_SIZE   = 9;
	static constexpr int SQUARE_COUNT = 3;


public:
	SudokuBoardEntity();

	void onReady(mk::Game& game) override;

	void load(const sudoku::SudokuBoard& board_data);

	mk::RectF getBounds() const;

	void onUpdate(mk::Game& game, float dt) override;

	std::optional<std::pair<sudoku::SudokuValue, sudoku::SudokuValue>> getSelectedTile() const;

private:
	void spawnTiles(mk::Game& game, float tile_size);
	void spawnLines(mk::Game& game, float tile_size);

	sf::Color even_color{}, odd_color{}, line_color{};
	float     square3x3_size{};

	mk::SpriteBatch*                                                          lines = nullptr;
	std::array<std::array<SudokuTileEntity*, BOARD_SIZE + 1>, BOARD_SIZE + 1> label_tiles{};
};
