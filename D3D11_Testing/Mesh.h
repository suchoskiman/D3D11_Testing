#pragma once
#include "testing.h"
#include "Renderer.h"
#include "Layout.h"

class Mesh
{
public:
	Mesh(const Vertex* vbuffer, UINT vcount, const WORD* ibuffer, UINT icount);
	Mesh(const Mesh& mesh);
	~Mesh();
	Mesh& operator=(const Mesh&& mesh) noexcept;

	Vertex* GetVertexBuffer()	{ return m_VertexBuffer; }
	UINT GetVertexBufferCount() const { return m_VertexBufferCount; }
	WORD* GetIndexBuffer() { return m_IndexBuffer; }
	UINT GetIndexBufferCount() const { return m_IndexBufferCount; }
	ComPtr<ID3D11Buffer> GetD3DVertexBuffer(ComPtr<ID3D11Device> device);
	ComPtr<ID3D11Buffer> GetD3DIndexBuffer(ComPtr<ID3D11Device> device);
	XMFLOAT4X4 GetModelMatrix();

	void Rotate(XMMATRIX rotation);

	static Mesh CreateCube(XMFLOAT3 color);
private:
	// vertex data
	Vertex* m_VertexBuffer;
	WORD* m_IndexBuffer;
	UINT m_VertexBufferCount;
	UINT m_IndexBufferCount;
	ComPtr<ID3D11Buffer> m_d3dVertexBuffer;
	ComPtr<ID3D11Buffer> m_d3dIndexBuffer;

	// transformation info
	XMFLOAT4X4 m_Translation;
	XMFLOAT4X4 m_Rotation;
	XMFLOAT4X4 m_Scale;
};
