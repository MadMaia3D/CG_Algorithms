#include "VoxelSpaceCamera.h"

void UpdateCamera(Camera * pCam, float deltaTime) {
	pCam->pos = Interpolate(pCam->pos, pCam->target_pos, 1.0f - exp(-deltaTime * pCam->position_acceleration));
	pCam->height = Interpolate(pCam->height, pCam->target_height, 1.0f - exp(-deltaTime * pCam->height_acceleration));
	pCam->angle = Interpolate(pCam->angle, pCam->target_angle, 1.0f - exp(-deltaTime * pCam->angle_acceleration));
	pCam->pitch = Interpolate(pCam->pitch, pCam->target_pitch, 1.0f - exp(-deltaTime * pCam->pitch_acceleration));
}

void CameraMoveForward(Camera *pCam, float speed, float deltaTime) {
	pCam->target_pos.x += cos(pCam->angle) * speed * deltaTime;
	pCam->target_pos.y += sin(pCam->angle) * speed * deltaTime;
}

void CameraMoveBackward(Camera *pCam, float speed, float deltaTime) {
	CameraMoveForward(pCam, -speed, deltaTime);
}

void CameraStrafeLeft(Camera *pCam, float speed, float deltaTime) {
	const Vec2 cameraDirection = Vec2(1.0f, 0.0f).Rotated(pCam->angle);
	const Vec2 cameraLeft = cameraDirection.Rotated90Clockwise();
	pCam->target_pos += cameraLeft * speed * deltaTime;
}

void CameraStrafeRight(Camera *pCam, float speed, float deltaTime) {
	CameraStrafeLeft(pCam, -speed, deltaTime);
}