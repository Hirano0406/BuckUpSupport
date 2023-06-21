//	[shadow.h]
#pragma once
#include "component.h"


class Shadow : public Component
{

private:

	ID3D11Buffer*				m_VertexBuffer{};
	DirectX::XMFLOAT3 m_Position{};
	ID3D11ShaderResourceView*	m_Texture{};

	float	m_Size = 1.0f;

public:
	using Component::Component;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetSize(float Size) { m_Size = Size; };
};
