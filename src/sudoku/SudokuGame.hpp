#pragma once
#include <vector>
#include <optional>
#include <cassert>

namespace sudoku {
	uint32_t getLoopCounter();

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
		void fill_random(std::optional<uint32_t> seed = {});

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
		 * @brief Tries to fills the board. If fails to fill the board state is incorrect.
		 * @return True if filled successfully, false otherwise.
		 */
		bool fill();

		/**
		 * @brief Checks is there is an only one way to solve this board.
		 */
		[[nodiscard]]
		bool isAmbiguous() const;


	private:
		struct IsAmbiguousResult {
			bool is_ambiguous;
			bool is_fillable;
		};

		[[nodiscard]]
		IsAmbiguousResult isAmbiguousImpl() const;

		std::array<SudokuSquare, 82> board{};
	};

	class SudokuGame {
	public:
		enum class Difficulty { EMPTY, NONE, EASY, NORMAL, HARD, EXPERT };

		explicit SudokuGame(Difficulty difficulty, std::optional<uint32_t> = {});

		[[nodiscard]]
		const SudokuBoard& getBoard() const;

		void fill();

	private:
		std::vector<SudokuBoard> history;

		SudokuBoard initial;
	};
}
