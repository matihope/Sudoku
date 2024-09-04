#pragma once
#include <vector>
#include <optional>
#include <cassert>

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

	struct SudokuBoard;

	struct SudokuSquare {
		SudokuSquare();
		std::optional<SudokuValue> main_digit{};
		std::array<bool, 10>       note_digits{};
		bool                       is_initial{};

	private:
		friend SudokuBoard;
		std::array<bool, 10> possible_moves{};  // 0 for convenience
		uint8_t              possible_move_count = 9;
	};

	class SudokuBoard {
	public:
		void fill_random(std::optional<int> seed = {});

		/**
		 * @brief Puts a value at column and row.
		 * @return true on success, false on failure.
		 */
		bool place_digit(SudokuValue column, SudokuValue row, SudokuValue value);

		bool can_place_digit(SudokuValue column, SudokuValue row, SudokuValue value);

		SudokuSquare& operator()(SudokuValue column, SudokuValue row);

		const SudokuSquare& operator()(SudokuValue column, SudokuValue row) const;

		friend std::ostream& operator<<(std::ostream& out, const SudokuBoard& board);

	private:
		/**
		 * @brief A function that helps to iterate over all squares in a 3x3 square,
		 * that should contain unique digits. This returns column or row numbers.
		 */
		static std::pair<SudokuValue, SudokuValue>   getColOrRowRangeForValue(SudokuValue value);
		std::array<std::array<SudokuSquare, 10>, 10> board{};

		bool fill(SudokuValue column, SudokuValue row, SudokuValue value);
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
