#pragma once

#include "testing.h"

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

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetViewMatrixInverse();

private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_LookAt;
	XMFLOAT3 m_UpDirection;
};

