#pragma once
#include <chrono>

class Timer {
public:
	Timer() {
		last = std::chrono::steady_clock::now();
	}
	float Mark() {
		std::chrono::steady_clock::time_point old = last;
		last = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = last - old;
		return elapsed.count();
	}
	float Peek() {
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}
private:
	std::chrono::steady_clock::time_point last;
};