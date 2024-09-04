
#include "SudokuGame.hpp"
#include "Random/Random.hpp"

namespace sudoku {
	SudokuGame::SudokuGame() {
		history.emplace_back();
		history.back().fill_random(0);
	}

	const SudokuBoard& SudokuGame::getBoard() const { return history.back(); }

	void SudokuBoard::fill_random(std::optional<int> seed) {
		// Set the seed
		if (seed.has_value())
			mk::Random::initSeed(*seed);
		else
			mk::Random::initRandom();
		// Fill the board
		for (SudokuValue col = 1; col <= 9; ++col) {
			for (SudokuValue row = 1; row <= 9; ++row)
				while (!place_digit(col, row, mk::Random::getInt(1, 9)))
					;
		}
	}

	bool SudokuBoard::place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		// Check column
		if (operator()(column, row).main_digit.has_value()) return false;
		for (SudokuValue check_col = 1; check_col <= 9; ++check_col) {
			auto&& square = operator()(check_col, row);
			if (square.main_digit.has_value()) {
				if (square.main_digit.value() == value) return false;
			}
		}
		// Check row
		for (SudokuValue check_row = 1; check_row <= 9; ++check_row) {
			auto&& square = operator()(column, check_row);
			if (square.main_digit.has_value()) {
				if (square.main_digit.value() == value) return false;
			}
		}
		// Check 3x3 square
		auto [row_begin, row_end] = getColOrRowRangeForValue(row);
		auto [col_begin, col_end] = getColOrRowRangeForValue(column);
		for (SudokuValue check_col = col_begin; check_col <= col_end; ++check_col) {
			for (SudokuValue check_row = col_begin; check_row <= row_end; ++check_row) {
				auto&& square = operator()(check_col, check_row);
				if (square.main_digit.has_value()) {
					if (square.main_digit.value() == value) return false;
				}
			}
		}
		// Set the value
		operator()(column, row).main_digit = value;
		return true;
	}

	std::pair<SudokuValue, SudokuValue> SudokuBoard::getColOrRowRangeForValue(SudokuValue value) {
		if (1 <= value && value <= 3) return { 1, 3 };
		if (4 <= value && value <= 6) return { 4, 6 };
		return { 7, 9 };
	}

	SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) {
		return board[column()][row()];
	}

	const SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) const {
		return board[column()][row()];
	}

	std::strong_ordering SudokuValue::operator<=>(const SudokuValue& other) const {
		return other() <=> value;
	}

	SudokuValue& SudokuValue::operator++() {
		value += 1;
		return *this;
	}

	bool SudokuValue::operator==(const SudokuValue& other) const { return value == other.value; }
}
