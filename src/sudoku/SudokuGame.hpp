#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <cassert>
#include <ostream>

namespace sudoku {
	class SudokuValue {
	public:
		template<class T>
		constexpr SudokuValue(T value): value(value) {
			assert(1 <= value && value <= 9);
		}

		uint8_t operator()() const { return value; }

		std::strong_ordering operator<=>(const SudokuValue& other) const;
		bool                 operator==(const SudokuValue& other) const;

		template<class T>
		requires std::is_arithmetic_v<T> std::strong_ordering operator<=>(const T& other) const {
			return value <=> other;
		}

		template<class T>
		requires std::is_arithmetic_v<T> bool operator==(const T& other) const {
			return value == other;
		}

		SudokuValue& operator++();

		friend std::ostream& operator<<(std::ostream& out, const SudokuValue& value);

	private:
		uint8_t value;
	};

	constexpr std::array<SudokuValue, 9> value_range = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	struct SudokuSquare {
		std::optional<SudokuValue> main_digit{};
		std::array<bool, 10>       note_digits{};
		bool                       is_initial{};
		bool                       is_correct = true;
	};

	class SudokuBoard {
	public:
		/**
		 * @brief Solves the board after setting the random to seed. If no seed
		 * is provided then random seed is used.
		 */

		void solve_random(std::optional<uint32_t> seed = {});

		/**
		 * @brief Puts a value at column and row. Fails if it's an illegal move.
		 * @return true on success, false on failure.
		 */
		bool place_digit(SudokuValue column, SudokuValue row, SudokuValue value);

		[[nodiscard]]
		bool can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value) const;

		SudokuSquare& operator()(SudokuValue column, SudokuValue row);

		const SudokuSquare& operator()(SudokuValue column, SudokuValue row) const;

		friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& board);

		/**
		 * @brief Tries to solve the board. If fails to solve the board state is incorrect.
		 * @return True if solved successfully, false otherwise.
		 */
		bool solve();

		/**
		 * @brief Checks is there is an only one way to solve this board.
		 */
		[[nodiscard]]
		bool isAmbiguous() const;

		/**
		 * @brief Checks if all squares have the is_correct flag true.
		 */
		bool isCorrect() const;


	private:
		struct IsAmbiguousResult {
			bool is_ambiguous;
			bool is_solvable;
		};

		[[nodiscard]]
		IsAmbiguousResult isAmbiguousImpl() const;

		std::array<SudokuSquare, 82> board{};
	};

	class SudokuGame {
	public:
		enum class Difficulty { EMPTY, NONE, EASY, NORMAL, HARD, EXPERT };

		explicit SudokuGame(Difficulty difficulty, std::optional<uint32_t> seed = {});

		[[nodiscard]]
		const SudokuBoard& getBoard() const;

		void solve();

		/**
		 * @brief Checks if all squares are filled.
		 */
		bool isOver() const;

		bool tryPlay(SudokuValue col, SudokuValue row, SudokuValue value);
		bool toggleNote(SudokuValue col, SudokuValue row, SudokuValue digit);

		/**
		 * @brief Reverts the last call of tryPlay() or toggleNote().
		 */
		bool undo();

		/**
		 * @brief A function that helps to iterate over all squares in a 3x3 square,
		 * that should contain unique digits. For each set, if the set contains value,
		 * then this set is the result. Sets: [1, 2, 3], [4, 5, 6], [7, 8, 9].
		 */
		static std::array<SudokuValue, 3> getColOrRowRangeForValue(sudoku::SudokuValue value);

	private:
		std::vector<SudokuBoard> history;

		SudokuBoard initial;
	};

	std::string difficultyToString(SudokuGame::Difficulty difficulty);
}
