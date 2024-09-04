
#include "SudokuGame.hpp"
#include "Random/Random.hpp"
#include <iostream>

namespace sudoku {
	SudokuGame::SudokuGame(SudokuGame::Difficulty difficulty) {
		history.emplace_back();
		history.back().fill_random(0);
		if (difficulty == Difficulty::EASY) {
			// delete 38 fields
			// check if unique
		}
	}

	const SudokuBoard& SudokuGame::getBoard() const { return history.back(); }

	void SudokuBoard::fill_random(std::optional<int> seed) {
		// Set the seed
		if (seed.has_value())
			mk::Random::initSeed(*seed);
		else
			mk::Random::initRandom();
		fill();
	}

	bool SudokuBoard::place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		if (!can_place_digit(column, row, value)) return false;
		operator()(column, row).main_digit = value;
		return true;
	}

	bool SudokuBoard::can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		if (operator()(column, row).main_digit.has_value()) return false;

		// Check columns
		for (SudokuValue check_col: value_range)
			if (operator()(check_col, row).main_digit == value) return false;

		// Check rows
		for (SudokuValue check_row: value_range)
			if (operator()(column, check_row).main_digit == value) return false;

		// Check 3x3 square
		for (SudokuValue check_col: getColOrRowRangeForValue(column))
			for (SudokuValue check_row: getColOrRowRangeForValue(row))
				if (operator()(check_col, check_row).main_digit == value) return false;
		return true;
	}

	std::array<SudokuValue, 3> SudokuBoard::getColOrRowRangeForValue(SudokuValue value) {
		if (1 <= value && value <= 3) return { 1, 2, 3 };
		if (4 <= value && value <= 6) return { 4, 5, 6 };
		return { 7, 8, 9 };
	}

	SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) {
		return board[column()][row()];
	}

	const SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) const {
		return board[column()][row()];
	}

	std::ostream& operator<<(std::ostream& out, const SudokuBoard& b) {
		out << std::string(19, '-') << '\n';
		for (SudokuValue row: value_range) {
			out << '|';
			for (SudokuValue col: value_range) {
				auto&& sq = b(col, row);
				if (sq.main_digit.has_value())
					out << sq.main_digit.value();
				else
					out << '0';

				if (col == 3 || col == 6 || col == 9)
					out << '|';
				else
					out << ' ';
			}
			out << "\n";
			if (row == 3 || row == 6) out << std::string(19, '-') << '\n';
		}
		out << std::string(19, '-') << '\n';
		return out;
	}

	std::strong_ordering SudokuValue::operator<=>(const SudokuValue& other) const {
		return value <=> other.value;
	}

	SudokuValue& SudokuValue::operator++() {
		value += 1;
		assert(value <= 9);
		return *this;
	}

	bool SudokuValue::operator==(const SudokuValue& other) const { return value == other.value; }

	std::ostream& operator<<(std::ostream& out, const SudokuValue& value) {
		out << (int) value();
		return out;
	}

	bool SudokuBoard::fill(SudokuValue column, SudokuValue row, SudokuValue value, int depth) {
		assert(depth <= 80);
		if (operator()(column, value).main_digit != value && !place_digit(column, row, value))
			return false;
		std::cerr << *this << '\n';

		if (column == 9 && row == 9) return true;

		std::array<SudokuValue, 9> order = value_range;
		mk::Random::shuffle(order.begin(), order.end());

		SudokuValue next_column = column;
		SudokuValue next_row    = row;
		if (row == 9) {
			next_row = 1;
			++next_column;
		} else {
			++next_row;
		}

		SudokuBoard cpy  = *this;
		int         test = 1;
		for (auto next_value: order) {
			std::cerr << "Trying at: " << depth << " value " << test++ << "/" << "9 - "
					  << next_value << "...\n";
			if (cpy.fill(next_column, next_row, next_value, depth + 1)) {
				*this = cpy;
				return true;
			} else {
				cpy(next_column, next_row).main_digit.reset();
			}
		}
		return false;
	}
}
