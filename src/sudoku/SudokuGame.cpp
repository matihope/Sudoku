#include "SudokuGame.hpp"
#include "Random/Random.hpp"
#include <iostream>

namespace sudoku {
	uint32_t loop_counter = 0;

	SudokuGame::SudokuGame(SudokuGame::Difficulty difficulty, std::optional<uint32_t> seed) {
		if (difficulty != Difficulty::EMPTY) {
			// Fill the board
			initial.solve_random(seed);

			// Blank some squares to make it fun
			history.push_back(initial);

			int to_remain = 9 * 9;
			if (difficulty == Difficulty::EASY) to_remain = 38;
			if (difficulty == Difficulty::NORMAL) to_remain = 35;
			if (difficulty == Difficulty::HARD) to_remain = 32;
			if (difficulty == Difficulty::EXPERT) to_remain = 28;

			auto&& board = history.back();
			for (int remaining = 9 * 9; remaining > to_remain; --remaining) {
				while (true) {
					// Try to remove a value, and if the board is ambiguous then undo.
					SudokuValue column = mk::Random::getInt(1, 9);
					SudokuValue row    = mk::Random::getInt(1, 9);
					auto&&      sq     = board(column, row);
					if (sq.main_digit.has_value()) {
						sq.main_digit.reset();
						if (board.isAmbiguous())
							sq.main_digit = initial(column, row).main_digit;
						else
							break;
					}
				}
			}
		} else {
			history.push_back(initial);
		}
	}

	const SudokuBoard& SudokuGame::getBoard() const { return history.back(); }

	void SudokuGame::solve() {
		auto board = history.back();
		board.solve();
		history.push_back(board);
	}

	void SudokuBoard::solve_random(std::optional<uint32_t> seed) {
		if (seed.has_value())
			mk::Random::initSeed(*seed);
		else
			mk::Random::initRandom();
		solve();
	}

	bool SudokuBoard::place_digit(SudokuValue column, SudokuValue row, SudokuValue value) {
		if (!can_place_digit(column, row, value)) return false;
		operator()(column, row).main_digit = value;

		// Remove notes

		// Self notes
		for (auto& note_digit: operator()(column, row).note_digits) note_digit = false;

		// Columns
		for (SudokuValue check_col: value_range)
			operator()(check_col, row).note_digits[value()] = false;

		// Rows
		for (SudokuValue check_row: value_range)
			operator()(column, check_row).note_digits[value()] = false;

		// 3x3 square
		for (SudokuValue check_col: SudokuGame::getColOrRowRangeForValue(column))
			for (SudokuValue check_row: SudokuGame::getColOrRowRangeForValue(row))
				operator()(check_col, check_row).note_digits[value()] = false;

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
		for (SudokuValue check_col: SudokuGame::getColOrRowRangeForValue(column))
			for (SudokuValue check_row: SudokuGame::getColOrRowRangeForValue(row))
				if (operator()(check_col, check_row).main_digit == value) return false;
		return true;
	}

	SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) {
		return board[column() * 9 + row() - 9];
	}

	const SudokuSquare& SudokuBoard::operator()(SudokuValue column, SudokuValue row) const {
		return board[column() * 9 + row() - 9];
	}

	std::ostream& operator<<(std::ostream& out, const SudokuBoard& board) {
		constexpr uint8_t width = 19;
		out << std::string(width, '-') << '\n';
		for (SudokuValue row: value_range) {
			out << '|';
			for (SudokuValue col: value_range) {
				auto&& sq = board(col, row);
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
			if (row == 3 || row == 6) out << std::string(width, '-') << '\n';
		}
		out << std::string(width, '-') << '\n';
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
		out << (int) value.value;
		return out;
	}

	uint32_t getLoopCounter() { return loop_counter; }

	bool SudokuBoard::solve() {
		std::array<SudokuValue, 9> order = value_range;
		mk::Random::shuffle(order.begin(), order.end());

		SudokuBoard cpy = *this;
		for (SudokuValue column: value_range) {
			for (SudokuValue row: value_range) {
				if (!operator()(column, row).main_digit.has_value()) {
					// We do the backtracking from here.
					for (SudokuValue next_value: order) {
						if (can_place_digit(column, row, next_value)) {
							cpy(column, row).main_digit = SudokuValue(next_value);
							if (cpy.solve()) {
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

	bool SudokuBoard::isAmbiguous() const { return isAmbiguousImpl().is_ambiguous; }

	SudokuBoard::IsAmbiguousResult SudokuBoard::isAmbiguousImpl() const {
		std::array<SudokuValue, 9> order = value_range;

		SudokuBoard cpy = *this;
		for (SudokuValue column: value_range) {
			for (SudokuValue row: value_range) {
				if (!operator()(column, row).main_digit.has_value()) {
					bool found = false;
					for (SudokuValue next_value: order) {
						loop_counter++;
						if (can_place_digit(column, row, next_value)) {
							cpy(column, row).main_digit = next_value;

							auto result = cpy.isAmbiguousImpl();
							if (result.is_ambiguous) return { true, result.is_solvable };
							if (result.is_solvable) {
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

	bool SudokuGame::tryPlay(SudokuValue col, SudokuValue row, SudokuValue value) {
		if (!history.back().isCorrect()) return false;

		auto original_value  = initial(col, row).main_digit;
		bool finally_correct = original_value == value;

		SudokuBoard board = history.back();

		if (board.place_digit(col, row, value)) {
			history.push_back(board);
			return true;
		}

		auto&& tile = board(col, row);
		if (!tile.is_initial) {
			for (auto&& note_digit: tile.note_digits) note_digit = false;
			tile.main_digit = value;
			tile.is_correct = false;
			history.push_back(board);
		}
		return false;
	}

	std::array<SudokuValue, 3> SudokuGame::getColOrRowRangeForValue(sudoku::SudokuValue value) {
		if (1 <= value && value <= 3) return { 1, 2, 3 };
		if (4 <= value && value <= 6) return { 4, 5, 6 };
		return { 7, 8, 9 };
	}

	bool SudokuGame::toggleNote(SudokuValue col, SudokuValue row, SudokuValue digit) {
		SudokuBoard board = history.back();
		if (!board(col, row).main_digit.has_value()) {
			auto&& note_digit = board(col, row).note_digits[digit()];
			note_digit        = !note_digit;
			history.push_back(board);
			return true;
		}
		return false;
	}

	bool SudokuGame::undo() {
		if (history.size() <= 1) return false;
		history.pop_back();
		return true;
	}

	bool SudokuGame::isOver() const {
		for (SudokuValue col: value_range) {
			for (SudokuValue row: value_range)
				if (!history.back()(col, row).main_digit.has_value()) return false;
		}
		return true;
	}

	bool SudokuBoard::isCorrect() const {
		for (SudokuValue col: value_range) {
			for (SudokuValue row: value_range)
				if (!operator()(col, row).is_correct) return false;
		}
		return true;
	}

	std::string difficultyToString(SudokuGame::Difficulty difficulty) {
		switch (difficulty) {
		case SudokuGame::Difficulty::EMPTY:
			return "Empty board";
		case SudokuGame::Difficulty::NONE:
			return "Full board";
		case SudokuGame::Difficulty::EASY:
			return "Easy";
		case SudokuGame::Difficulty::NORMAL:
			return "Normal";
		case SudokuGame::Difficulty::HARD:
			return "Hard";
		case SudokuGame::Difficulty::EXPERT:
			return "Expert";
			break;
		}
	}
}
