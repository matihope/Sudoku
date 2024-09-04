
#include "SudokuGame.hpp"
#include "Random/Random.hpp"
#include <iostream>

namespace sudoku {
	int looped = 0;

	SudokuGame::SudokuGame(SudokuGame::Difficulty difficulty) {
		history.emplace_back();
		history.back().fill_random();

		int remaining = 81;
		if (difficulty == Difficulty::EASY) remaining = 38;
		if (difficulty == Difficulty::EXPERT) remaining = 28;

		SudokuBoard backup = history.back();
		for (int i = 0; i < 81 - remaining; i++) {
			while (true) {
				SudokuValue column = mk::Random::getInt(1, 9);
				SudokuValue row    = mk::Random::getInt(1, 9);
				auto&&      sq     = history.back()(column, row);
				if (sq.main_digit.has_value()) {
					sq.main_digit.reset();
					if (history.back().is_ambiguous())
						sq.main_digit = backup(column, row).main_digit;
					else
						break;
				}
			}
		}
	}

	const SudokuBoard& SudokuGame::getBoard() const { return history.back(); }

	void SudokuGame::fill() {
		history.back().fill();
	}

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

	bool
		SudokuBoard::can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value) const {
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
					out << ' ';

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

	int getLooped() { return looped; }

	bool SudokuBoard::fill(int depth) {
		static int digit_tried = 0;
		//		if (depth == 0) digit_tried = 0;

		std::array<SudokuValue, 9> order = value_range;
		mk::Random::shuffle(order.begin(), order.end());

		SudokuBoard cpy = *this;
		for (SudokuValue column: value_range) {
			for (SudokuValue row: value_range) {
				if (!operator()(column, row).main_digit.has_value()) {
					// We do the backtracking from here.
					int test = 1;
					for (SudokuValue next_value: order) {
//						std::cerr << "Trying at: " << depth << " value " << test++ << "/" << "9 - "
//								  << next_value << ", conducted test: " << ++digit_tried << '\n';

						if (can_place_digit(column, row, next_value)) {
							cpy(column, row).main_digit = next_value;
							if (cpy.fill(depth + 1)) {
								*this = cpy;
								return true;
							}
						}
					}
					return false;
				}
			}
		}
		return true;
	}

	bool SudokuBoard::can_fill() const {
		std::array<SudokuValue, 9> order = value_range;
		mk::Random::shuffle(order.begin(), order.end());

		SudokuBoard cpy = *this;
		for (SudokuValue column: value_range) {
			for (SudokuValue row: value_range) {
				if (!operator()(column, row).main_digit.has_value()) {
					for (SudokuValue next_value: order) {
						looped++;
						if (can_place_digit(column, row, next_value)) {
							cpy(column, row).main_digit = next_value;
							if (cpy.can_fill()) return true;
						}
					}
					return false;
				}
			}
		}
		return true;
	}

	bool SudokuBoard::is_ambiguous() const { return is_ambiguous2().first; }

	std::pair<bool, bool> SudokuBoard::is_ambiguous2() const {
		std::array<SudokuValue, 9> order = value_range;
		mk::Random::shuffle(order.begin(), order.end());

		SudokuBoard cpy = *this;
		for (SudokuValue column: value_range) {
			for (SudokuValue row: value_range) {
				if (!operator()(column, row).main_digit.has_value()) {
					bool found = false;
					for (SudokuValue next_value: order) {
						looped++;
						if (can_place_digit(column, row, next_value)) {
							cpy(column, row).main_digit = next_value;

							auto [amb, filled] = cpy.is_ambiguous2();
							if (amb) return { true, filled };
							if (filled) {
								if (found) return { true, true };
								found = true;
							}
						}
					}
					return { false, found };
				}
			}
		}
		return { false, true };
	}
}
