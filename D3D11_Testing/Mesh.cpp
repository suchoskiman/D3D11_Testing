#include "Mesh.h"

inline Mesh::Mesh(const Vertex* vbuffer, UINT vcount, const WORD* ibuffer, UINT icount):
	m_VertexBuffer(NULL), m_IndexBuffer(NULL), m_VertexBufferCount(0), m_IndexBufferCount(0),
	m_d3dVertexBuffer(NULL), m_d3dIndexBuffer(NULL)
{
	m_VertexBuffer = new Vertex[vcount];
	m_IndexBuffer = new WORD[icount];
	m_VertexBufferCount = vcount;
	m_IndexBufferCount = icount;

	memcpy(m_VertexBuffer, vbuffer, vcount * sizeof(Vertex));
	memcpy(m_IndexBuffer, ibuffer, icount * sizeof(WORD));

	// sets all matrices to the identity (no transformations)
	XMStoreFloat4x4(&m_Translation, XMMatrixIdentity());
	XMStoreFloat4x4(&m_Rotation, XMMatrixIdentity());
	XMStoreFloat4x4(&m_Scale, XMMatrixIdentity());
}

Mesh::Mesh(const Mesh& mesh):
	m_VertexBuffer(NULL), m_IndexBuffer(NULL), m_VertexBufferCount(0),
	m_IndexBufferCount(0), m_d3dVertexBuffer(NULL), m_d3dIndexBuffer(NULL)
{
	m_VertexBuffer = new Vertex[mesh.m_VertexBufferCount];
	m_IndexBuffer = new WORD[mesh.m_IndexBufferCount];
	memcpy(m_VertexBuffer, mesh.m_VertexBuffer, mesh.m_VertexBufferCount * sizeof(Vertex));
	memcpy(m_IndexBuffer, mesh.m_IndexBuffer, mesh.m_IndexBufferCount * sizeof(WORD));

	m_VertexBufferCount = mesh.GetVertexBufferCount();
	m_IndexBufferCount = mesh.GetIndexBufferCount();

	XMStoreFloat4x4(&m_Translation, XMMatrixIdentity());
	XMStoreFloat4x4(&m_Rotation, XMMatrixIdentity());
	XMStoreFloat4x4(&m_Scale, XMMatrixIdentity());
}

Mesh::~Mesh()
{

}

Mesh& Mesh::operator=(const Mesh&& mesh) noexcept
{
	if (m_VertexBuffer != NULL)
	{
		delete[] m_VertexBuffer;
	}
	if (m_IndexBuffer)
	{
		delete[] m_IndexBuffer;
	}

	m_VertexBuffer = mesh.m_VertexBuffer;
	m_IndexBuffer = mesh.m_IndexBuffer;
	m_VertexBufferCount = mesh.m_VertexBufferCount;
	m_IndexBufferCount = mesh.m_IndexBufferCount;

	return *this;
}

ComPtr<ID3D11Buffer> Mesh::GetD3DVertexBuffer(ComPtr<ID3D11Device> device)
{
	if (m_d3dVertexBuffer != NULL) return m_d3dVertexBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Vertex) * m_VertexBufferCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = m_VertexBuffer;

	ComPtr<ID3D11Buffer> vBuffer;
	device->CreateBuffer(&bufferDesc, &resourceData, vBuffer.GetAddressOf());

	m_d3dVertexBuffer = vBuffer;
	return vBuffer;
}

ComPtr<ID3D11Buffer> Mesh::GetD3DIndexBuffer(ComPtr<ID3D11Device> device)
{
	if (m_d3dIndexBuffer != NULL) return m_d3dIndexBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(WORD) * m_IndexBufferCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = m_IndexBuffer;

	ComPtr<ID3D11Buffer> iBuffer;
	device->CreateBuffer(&bufferDesc, &resourceData, iBuffer.GetAddressOf());

	m_d3dIndexBuffer = iBuffer;
	return iBuffer;
}

XMFLOAT4X4 Mesh::GetModelMatrix()
{
	XMFLOAT4X4 output;
	XMMATRIX translation = XMLoadFloat4x4(&m_Translation);
	XMMATRIX rotation = XMLoadFloat4x4(&m_Rotation);
	XMMATRIX scale = XMLoadFloat4x4(&m_Scale);

	XMStoreFloat4x4(&output, XMMatrixMultiply(scale, XMMatrixMultiply(rotation, translation)));
	return output;
}

void Mesh::Rotate(XMMATRIX rotation)
{
	XMStoreFloat4x4(&m_Rotation, XMMatrixMultiply(XMLoadFloat4x4(&m_Rotation), rotation));
}

Mesh Mesh::CreateCube(XMFLOAT3 color)
{
	Vertex tempVBuffer[8] =
	{
		{ { -1.0f, -1.0f, -1.0f },	color },		// 0
		{ { 1.0f, -1.0f, -1.0f },	color },		// 1
		{ { -1.0f, 1.0f, -1.0f },	color },		// 2
		{ { 1.0f, 1.0f, -1.0f },	color },		// 3
		{ { -1.0f, -1.0f, 1.0f },	color },		// 4
		{ { 1.0f, -1.0f, 1.0f },	color },		// 5
		{ { -1.0f, 1.0f, 1.0f },	color },		// 6
		{ { 1.0f, 1.0f, 1.0f },		color }			// 7
	};
	WORD tempIBuffer[36] =
	{
		0, 2, 3,
		0, 3, 1,
		1, 3, 7,
		1, 7, 5,
		5, 7, 6,
		5, 6, 4,
		4, 6, 2,
		4, 2, 0,
		2, 6, 7,
		2, 7, 3,
		1, 5, 4,
		1, 4, 0
	};
	
	return Mesh(tempVBuffer, 8, tempIBuffer, 36);
}
