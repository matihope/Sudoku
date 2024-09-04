#pragma once
#include <vector>
#include <optional>
#include <cassert>
#include "Random/Random.hpp"

namespace sudoku {
	int getLooped();

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

	struct SudokuBoard;

	struct SudokuSquare {
		std::optional<SudokuValue> main_digit{};
		std::array<bool, 10>       note_digits{};
		bool                       is_initial{};
	};

	class SudokuBoard {
	public:
		void fill_random(std::optional<int> seed = {});

		/**
		 * @brief Puts a value at column and row.
		 * @return true on success, false on failure.
		 */
		bool place_digit(SudokuValue column, SudokuValue row, SudokuValue value);

		bool can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value) const;

		SudokuSquare& operator()(SudokuValue column, SudokuValue row);

		const SudokuSquare& operator()(SudokuValue column, SudokuValue row) const;

		friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& board);

		/**
		 * @brief Tries to fills the board. If fails to fill the board state is incorrect.
		 * @return True if filled successfully, false otherwise.
		 */
		bool fill(int depth = 0);

		/**
		 * @brief Checks if it's possible to solve the board.
		 */
		[[nodiscard]]
		bool can_fill() const;

		/**
		 * @brief Checks is there is an only one way to solve this board.
		 */
		[[nodiscard]]
		bool is_ambiguous() const;

	private:
		/**
		 * @brief A function that helps to iterate over all squares in a 3x3 square,
		 * that should contain unique digits. This returns column or row numbers.
		 */
		static std::array<SudokuValue, 3>            getColOrRowRangeForValue(SudokuValue value);
		std::array<std::array<SudokuSquare, 10>, 10> board{};
	};

	class SudokuGame {
	public:
		enum class Difficulty { NONE, EASY, EXPERT };

		explicit SudokuGame(Difficulty difficulty);

		[[nodiscard]]
		const SudokuBoard& getBoard() const;


	private:
		std::vector<SudokuBoard> history;
	};
}
