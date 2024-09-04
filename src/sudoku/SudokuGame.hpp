#pragma once
#include <vector>
#include <optional>
#include <cassert>

namespace sudoku {
	class SudokuValue {
	public:
		template<class T>
		SudokuValue(T value): value(value) {
			assert(1 <= value && value <= 9);
		}

		uint8_t operator()() const { return value; }

		std::strong_ordering operator<=>(const SudokuValue& other) const;
		bool                 operator==(const SudokuValue& other) const;

		template<class T>
		requires std::is_arithmetic_v<T> std::strong_ordering operator<=>(const T& other) const {
			return other <=> value;
		}

		template<class T>
		requires std::is_arithmetic_v<T> bool operator==(const T& other) const {
			return other == value;
		}

		SudokuValue& operator++();

	private:
		uint8_t value;
	};

	extern const SudokuValue value_range[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	struct SudokuSquare {
		std::optional<SudokuValue> main_digit{};
		std::array<bool, 9>        note_digits{};
		bool                       is_initial{};
	};

	struct SudokuBoard {
		void fill_random(std::optional<int> seed = {});

		/**
		 * @brief Puts a value at column and row.
		 * @return true on success, false on failure.
		 */
		bool place_digit(SudokuValue column, SudokuValue row, SudokuValue value);

		SudokuSquare& operator()(SudokuValue column, SudokuValue row);

		const SudokuSquare& operator()(SudokuValue column, SudokuValue row) const;

	private:
		/**
		 * @brief A function that helps to iterate over all squares in a 3x3 square,
		 * that should contain unique digits. This returns column or row numbers.
		 */
		static std::pair<SudokuValue, SudokuValue> getColOrRowRangeForValue(SudokuValue value);
		std::array<std::array<SudokuSquare, 9>, 9> board{};
	};

	class SudokuGame {
	public:
		SudokuGame();

		[[nodiscard]]
		const SudokuBoard& getBoard() const;

	private:
		std::vector<SudokuBoard> history;
	};
}
