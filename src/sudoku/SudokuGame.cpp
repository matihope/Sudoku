
#include "SudokuGame.hpp"
#include "Random/Random.hpp"
#include <iostream>

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
		for (SudokuValue value: value_range) {
			for (SudokuValue _: value_range) {
				while (!place_digit(mk::Random::getInt(1, 9), mk::Random::getInt(1, 9), value));
				std::cout << *this << '\n';
			}
		}
	}

	bool SudokuBoard::place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		if (!can_place_digit(column, row, value)) return false;

		// Set columns
		for (SudokuValue check_col: value_range) {
			auto&& sq                  = operator()(check_col, row);
			sq.possible_moves[value()] = false;
			sq.possible_move_count--;
		}
		// Set rows
		for (SudokuValue check_row: value_range) {
			auto&& sq                  = operator()(column, check_row);
			sq.possible_moves[value()] = false;
			sq.possible_move_count--;
		}

		// Set 3x3 square
		auto [row_begin, row_end] = getColOrRowRangeForValue(row);
		auto [col_begin, col_end] = getColOrRowRangeForValue(column);
		for (SudokuValue check_col = col_begin; check_col <= col_end; ++check_col) {
			for (SudokuValue check_row = row_begin; check_row <= row_end; ++check_row) {
				if (check_col != column && check_row != row) {
					operator()(check_col, check_row).possible_moves[value()] = false;
					operator()(check_col, check_row).possible_move_count--;
				}
			}
		}

		// Set the value
		auto&& me     = operator()(column, row);
		me.main_digit = value;
		me.possible_moves.fill(false);
		me.possible_move_count = 0;

		return true;
	}

	bool SudokuBoard::can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		auto&& me = operator()(column, row);
		if (me.main_digit.has_value()) return false;

		// Check me
		if (!me.possible_moves[value()]) return false;

		// Check columns
		for (SudokuValue check_col: value_range) {
			if (check_col != column) {
				auto&& sq = operator()(check_col, row);
				if (!sq.main_digit.has_value() && sq.possible_moves[value()]
				    && sq.possible_move_count <= 1)
					return false;
			}
		}
		// Check rows
		for (SudokuValue check_row: value_range) {
			if (check_row != row) {
				auto&& sq = operator()(column, check_row);
				if (!sq.main_digit.has_value() && sq.possible_moves[value()]
				    && sq.possible_move_count <= 1)
					return false;
			}
		}

		// Check 3x3 square
		auto [row_begin, row_end] = getColOrRowRangeForValue(row);
		auto [col_begin, col_end] = getColOrRowRangeForValue(column);
		for (SudokuValue check_col = col_begin; check_col <= col_end; ++check_col) {
			for (SudokuValue check_row = row_begin; check_row <= row_end; ++check_row) {
				// && not || because the other were already checked
				if (check_row != row && check_col != column) {
					auto&& sq = operator()(check_col, check_row);
					if (!sq.main_digit.has_value() && sq.possible_moves[value()]
					    && sq.possible_move_count <= 1)
						return false;
				}
			}
		}
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
		return *this;
	}

	bool SudokuValue::operator==(const SudokuValue& other) const { return value == other.value; }

	std::ostream& operator<<(std::ostream& out, const SudokuValue& value) {
		out << (int) value();
		return out;
	}

	SudokuSquare::SudokuSquare() { possible_moves.fill(true); }

	bool SudokuBoard::fill(SudokuValue column, SudokuValue row, SudokuValue value) {
		// operator()(column, row).
		// std::array<SudokuValue, 9> order = value_range;
		// mk::Random::shuffle(order.begin(), order.end());
		// for (auto& value: order) {

		// }
	}
}
