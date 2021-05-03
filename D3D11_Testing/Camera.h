#pragma once

#include "testing.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Camera
{
public:
	Camera(XMFLOAT3 position, XMFLOAT3 lookAt, XMFLOAT3 upDirection = { 0.0f, 1.0f, 0.0f });
	~Camera();

	XMFLOAT3 GetLookAt();
	void SetLookAt(XMFLOAT3 newLookAt);

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPosition);

	XMFLOAT3 GetUpDirection();
	void SetUpDirection(XMFLOAT3 newUpDirection);

	XMMATRIX GetCameraTransform();
	XMMATRIX GetCameraTransformInverse();

private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_LookAt;
	XMFLOAT3 m_UpDirection;
};

