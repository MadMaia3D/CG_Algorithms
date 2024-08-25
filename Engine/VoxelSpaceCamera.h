#pragma once
#include "Graphics.h"

struct Camera {
	Vec2 pos;
	float height = 100.0f;
	float angle = -PI / 2;
	float pitch = 250.0f;
	float zfar = 512.0f;

	float position_acceleration = 3.0f;
	float height_acceleration = 5.0f;
	float angle_acceleration = 5.0f;
	float pitch_acceleration = 5.0f;

	Vec2 target_pos = pos;
	float target_height = 100.0f;
	float target_angle = -PI / 2;
	float target_pitch = 250.0f;

	float FOV = PI / 2.0f;
};

void UpdateCamera(Camera *pCam, float deltaTime);
void CameraMoveForward(Camera *pCam, float speed, float deltaTime);
void CameraMoveBackward(Camera *pCam, float speed, float deltaTime);
void CameraStrafeLeft(Camera *pCam, float speed, float deltaTime);
void CameraStrafeRight(Camera *pCam, float speed, float deltaTime);