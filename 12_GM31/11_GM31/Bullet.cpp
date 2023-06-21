//	Bulletオブジェクト[Bullet.cpp]
#include "Bullet.h"
#include "modelRenderer.h"
#include "shader.h"
#include "shadow.h"
#include "scene.h"
#include "manager.h"
#include "enemy.h"
#include "Explosion.h"
#include "score.h"

void Bullet::Load()
{
	ModelRenderer::Preload("asset/model/bullet.obj");
}

void Bullet::UnLoad()
{

}

void Bullet::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/bullet.obj");
	AddComponent<Shadow>()->SetSize(0.5f);
}

void Bullet::Update()
{
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	float Length;
	DirectX::XMStoreFloat(&Length,
		DirectX::XMVector3LengthSq(
			DirectX::XMLoadFloat3(&m_Position)));

	if (Length > (100.0f * 100.0f))
	{
		SetDestroy();
	}

	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();
	for (Enemy* enemy : enemyList)
	{
		DirectX::XMFLOAT3 position = enemy->GetPosition();
		DirectX::XMFLOAT3  direction;
		direction.x = position.x - m_Position.x;
		direction.y = position.y - m_Position.y;
		direction.z = position.z - m_Position.z;
		DirectX::XMStoreFloat(&Length,
			DirectX::XMVector3LengthSq(
				DirectX::XMLoadFloat3(&direction)));
		if (Length < (2.0f * 2.0f))
		{
			Explosion* explosion = scene->AddGameObject<Explosion>(3);
			explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));
			explosion->SetPosition(DirectX::XMFLOAT3(position.x,
				position.y + 1.0f, position.z));

			Score* score = scene->GetGameObject<Score>();
			score->AddCount(10);

			enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
}
