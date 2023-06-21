//	EnemyƒNƒ‰ƒX [enemy.cpp]
#include "enemy.h"
#include "shader.h"
#include "modelRenderer.h"
#include "shadow.h"

void Enemy::Init()
{
	AddComponent<Shader>()->Load(
		"shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load(
		"asset/model/enemy.obj");
	AddComponent<Shadow>()->SetSize(2.0f);
}

