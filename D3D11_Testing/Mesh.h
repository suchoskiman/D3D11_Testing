#pragma once
#include "testing.h"

template <typename t_VertexLayout>
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh& operator=(const Mesh&& mesh);

	static Mesh& CreateCube();
private:
	t_VertexLayout* m_VertexBuffer;
	int* m_IndexBuffer;
	int m_VertexBufferCount;
	int m_IndexBufferCount;
};

