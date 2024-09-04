//
// Created by mateusz on 9/24/23.
//

#include "Random.hpp"

namespace mk {
	Random &Random::get() {
		static Random instance;
		return instance;
	}

	void Random::initSeed(uint seed) {
		auto &me = Random::get();
		me.generator.seed(seed);
	}

	void Random::initRandom() {
		auto &me     = Random::get();
		me.generator = std::default_random_engine(me.randomDevice());
	}

}  // namespace mk
