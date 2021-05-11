#include "Camera.h"
#include "testing.h"

Camera::Camera(XMFLOAT3 position, XMFLOAT3 lookAt, XMFLOAT3 upDirection):
	m_Position(position), m_LookAt(lookAt), m_UpDirection(upDirection)
{

}

Camera::~Camera()
{
	
}

XMFLOAT3 Camera::GetLookAt()
{
	return { 0.0f, 0.0f, 0.0f };
}
void Camera::SetLookAt(XMFLOAT3 newLookAt)
{

}

XMFLOAT3 Camera::GetPosition()
{
	return { 0.0f, 0.0f, 0.0f };
}
void Camera::SetPosition(XMFLOAT3 newPosition)
{

}

XMFLOAT3 Camera::GetUpDirection()
{
	return { 0.0f, 0.0f, 0.0f };
}
void Camera::SetUpDirection(XMFLOAT3 newUpDirection)
{

}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	XMFLOAT4X4 output;
	XMStoreFloat4x4(&output, XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_LookAt),
		XMLoadFloat3(&m_UpDirection)));
	return output;
}

XMFLOAT4X4 Camera::GetViewMatrixInverse()
{
	XMFLOAT4X4 output;
	XMStoreFloat4x4(&output, XMMatrixIdentity());
	return output;
}