#pragma once
#include <chrono>

class FrameTimer {
public:
	FrameTimer() {
		last = std::chrono::steady_clock::now();
	};
	float Mark() {
		using namespace std::chrono;
		const steady_clock::time_point old = last;
		last = steady_clock::now();
		const duration<float> elapsed = last - old;
		return elapsed.count();
	}
private:
	std::chrono::steady_clock::time_point last;
};