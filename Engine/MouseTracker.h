#pragma once
#include "Vector2.h"

class MouseTracker {
public:
	void StartTracking(const Vec2& currentMousePos) {
		lastPosition = currentMousePos;
		isTracking = true;
	}

	void StopTracking() {
		isTracking = false;
	}

	Vec2 Track(const Vec2& currentMousePos) {
		if (!isTracking) {
			return { 0.0f, 0.0f };
		}
		Vec2 difference = lastPosition - currentMousePos;
		lastPosition = currentMousePos;
		return difference;
	}

	bool IsTracking() const {
		return isTracking;
	}

private:
	bool isTracking = false;
	Vec2 lastPosition = {0.0f, 0.0f};
};