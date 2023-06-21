// Tankオブジェクト [tank.h]
#pragma once
#include "gameObject.h"

class Tank : public GameObject
{
private:
	GameObject* m_Child{};
	float       m_MoveTime{};
	float       m_BulletTime{};

public:
	void Init() override;
	void Update() override;
};
