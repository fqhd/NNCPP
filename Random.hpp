#pragma once
#include <random>
#include <ctime>

std::mt19937 gnrtr_mt19937_rndmhpp;


class Random{
public:

    static void seed(uint32_t seed) {
        gnrtr_mt19937_rndmhpp.seed(seed);
    }

	static int uniformInt(int min, int max) {
		std::uniform_int_distribution<int> number(min, max);
		return number(gnrtr_mt19937_rndmhpp);
	}

    static float uniformFloat(float min, float max) {
        std::uniform_real_distribution<float> number(min, max);
        return number(gnrtr_mt19937_rndmhpp);
    }


};