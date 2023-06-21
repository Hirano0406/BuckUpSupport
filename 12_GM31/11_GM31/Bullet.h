#pragma once
#include "gameObject.h"

class Bullet : public GameObject
{
private:
	DirectX::XMFLOAT3 m_Velocity{};

public:
	static void Load();
	static void UnLoad();

	void Init() override;
	void Update() override;

	void SetVelocity(DirectX::XMFLOAT3 Velocity) { m_Velocity = Velocity; }
};