#pragma once

#include <algorithm>
#include <concepts>
#include <random>

namespace mk {

	typedef unsigned int uint;

	class Random {
		Random() = default;
		static Random& get();

		std::default_random_engine generator;
		std::random_device         randomDevice;

	public:
		// singleton stuff
		Random(const Random&)         = delete;
		void operator=(const Random&) = delete;

		static void initSeed(uint seed);
		static void initRandom();

		template<class T>
		requires std::integral<T> static T getInt(T start, T end) {
			auto& me = Random::get();
			return std::uniform_int_distribution<T>(start, end)(me.generator);
		}

		template<class T>
		requires std::floating_point<T> static T getReal(T start, T end) {
			auto& me = Random::get();
			return std::uniform_real_distribution<T>(start, end)(me.generator);
		}

		template<class T>
		static T getNormalDist(T mean, T stddev) {
			auto& me = Random::get();
			return std::normal_distribution<T>(mean, stddev)(me.generator);
		}

		template<class RandomIt>
		static void shuffle(RandomIt first, RandomIt last) {
			auto& me = Random::get();
			std::shuffle(first, last, me.generator);
		}
	};

}  // namespace mk
