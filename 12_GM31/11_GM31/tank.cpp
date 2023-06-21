// Tankオブジェクト [tank.cpp]
#include "tank.h"

#include "shader.h"
#include "modelRenderer.h"
#include "shadow.h"

#include "manager.h"

#include "bullet.h"

void Tank::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/battery1.obj");
	AddComponent<Shadow>()->SetSize(3.0f);

	m_Child = AddChild<GameObject>();
	m_Child->AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	m_Child->AddComponent<ModelRenderer>()->Load("asset/model/battery2.obj");
}

void Tank::Update()
{
	m_MoveTime += 1.0f / 60.0f;
	m_Position.z += cosf(m_MoveTime) * 0.1f;

	DirectX::XMFLOAT3 rotation = m_Child->GetRotation();
	rotation.y += 0.02f;
	m_Child->SetRotation(rotation);

	//弾発射
	m_BulletTime += 1.0f / 60.0f;

	if (m_BulletTime > 1.0f)
	{
		m_BulletTime -= 1.0f;

		DirectX::XMFLOAT3 forward = m_Child->GetForward();

		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>();
		bullet->SetPosition(DirectX::XMFLOAT3(m_Position.x + 0.0f,
			m_Position.y + 1.5f, m_Position.z + 0.0f));
		bullet->SetVelocity(DirectX::XMFLOAT3(forward.x * -0.5f,
			forward.y * -0.5f, forward.z * -0.5f));
	}
}
