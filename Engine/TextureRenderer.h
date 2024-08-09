#pragma once
#include "Graphics.h"

#define CLAMP_TO_BORDER_Y 0b0001
#define CLAMP_TO_BORDER_X 0b0010
#define CLAMP_TO_EDGE_X 0b0100
#define CLAMP_TO_EDGE_Y 0b1000

struct CameraTransform {
	Vec2 cameraPos = { 0.0f, 0.0f };
	float zoomLevel = 2.0f;
};

void TransformCamera(CameraTransform *pCam, Vec2 deltaPosition, float deltaZoom);

Vec2 TransformToCameraSpace(Vec2 point, const CameraTransform *ct);

void RenderTexture(CameraTransform &ct, const Surface &texture, Graphics& gfx, unsigned int options = 0);