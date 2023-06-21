#pragma once

#include "gameObject.h"

class Player : public GameObject
{

private:
	DirectX::XMFLOAT3 m_Velocity{};
	class Audio*        m_SE{};


public:
	void Init() override;
	void Update() override;

};
