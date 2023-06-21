#include "manager.h"
#include "renderer.h"
#include "cube.h"
#include "shader.h"
#include "player.h"
#include "input.h"
#include "modelRenderer.h"
#include"shadow.h"
#include "Bullet.h"
#include "box.h"
#include "cylinder.h"
#include "goal.h"
#include "explosion.h"
#include "audio.h"



void Player::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/player.obj");
	AddComponent<Shadow>()->SetSize(1.5f);
	m_SE = AddComponent<Audio>();
	m_SE->Load("asset/audio/wan.wav");
	SetScale(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f));
}




void Player::Update()
{
	DirectX::XMFLOAT3 oldPosition = m_Position;

	if (Input::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.02f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Rotation.y += 0.02f;
	}


	DirectX::XMFLOAT3 forward = GetForward();

	if (Input::GetKeyPress('W'))
	{
		m_Position.x += forward.x * 0.1f;
		m_Position.y += forward.y * 0.1f;
		m_Position.z += forward.z * 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position.x -= forward.x * 0.1f;
		m_Position.y -= forward.y * 0.1f;
		m_Position.z -= forward.z * 0.1f;
	}

	//ジャンプ
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		m_Velocity.y = 0.35f;
	}

	//重力
	m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//移動
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	//接地
	float groundHeight = 0.0f;

	Scene* scene = Manager::GetScene();
	std::vector<Cylinder*> cylinderList = scene->GetGameObjects<Cylinder>();
	for (Cylinder* cylinder : cylinderList)
	{
		DirectX::XMFLOAT3 position = cylinder->GetPosition();
		DirectX::XMFLOAT3 scale = cylinder->GetScale();
		DirectX::XMFLOAT3 direction =
			DirectX::XMFLOAT3(m_Position.x - position.x,
				0.0f, m_Position.z - position.z);
		float length;
		DirectX::XMStoreFloat(&length,
			DirectX::XMVector3Length(DirectX::XMLoadFloat3(&direction)));
		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}
	}
	std::vector<Box*> boxList = scene->GetGameObjects<Box>();
	for (Box* box : boxList)
	{
		DirectX::XMFLOAT3 position = box->GetPosition();
		DirectX::XMFLOAT3 scale = box->GetScale();
		if (position.x - scale.x - 0.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.5f &&
			position.z - scale.z - 0.5f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.5f)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}
	Goal* goal = scene->GetGameObject<Goal>();
	if (goal)
	{
		DirectX::XMFLOAT3 position = goal->GetPosition();
		DirectX::XMFLOAT3 scale = goal->GetScale();
		if (position.x - scale.x - 0.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.5f &&
			position.y - scale.y - 0.5f < m_Position.y &&
			m_Position.y + 1.5f < position.y + scale.y + 0.5f &&
			position.z - scale.z - 0.5f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.5f)
		{
			scene->AddGameObject<Explosion>(2)->SetPosition(position);
			goal->SetDestroy();
		}
	}
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
	//	弾発射
	if (Input::GetKeyTrigger('K'))
	{
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>();
		bullet->SetPosition(DirectX::XMFLOAT3(m_Position.x,
			m_Position.y + 1.0f, m_Position.z));
		bullet->SetVelocity(DirectX::XMFLOAT3(forward.x * 0.5f,
			forward.y * 0.5f, forward.z * 0.5f));
		m_SE->Play();
	}

}
