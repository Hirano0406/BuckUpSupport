#include "manager.h"
#include "renderer.h"
#include "camera.h"

#include "input.h"
#include "scene.h"
#include "player.h"

void Camera::Init()
{
	m_Position = DirectX::XMFLOAT3( 0.0f, 5.0f, -10.0f );
	m_Target = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
}



void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = 
		scene->GetGameObject<Player>();
	DirectX::XMFLOAT3 playerPos =
		player->GetPosition();
	DirectX::XMFLOAT3 playerFwd =
		player->GetForward();

	if (Input::GetKeyTrigger('R'))
		m_mode = (m_mode + 1) % 3;

	switch (m_mode)
	{
	case 0:
		//	サード パーソン ビュー
		m_Target.x = playerPos.x;
		m_Target.y = playerPos.y + 1.0f;
		m_Target.z = playerPos.z;
		m_Position.x = 
			playerPos.x - 5.0f * playerFwd.x;
		m_Position.y = 
			playerPos.y - 5.0f * playerFwd.y + 3.0f;
		m_Position.z = 
			playerPos.z - 5.0f * playerFwd.z;
		break;
	case 1:
		//	ファースト パーソン ビュー
		m_Position.x = 
			playerPos.x + 0.416f * playerFwd.x;
		m_Position.y = 
			playerPos.y + 0.416f * playerFwd.y + 1.778f;
		m_Position.z = 
			playerPos.z + 0.416f * playerFwd.z;
		m_Target.x = m_Position.x + playerFwd.x;
		m_Target.y = m_Position.y + playerFwd.y;
		m_Target.z = m_Position.z + playerFwd.z;
		break;
	case 2:
		m_Position.x = 0.0f;
		m_Position.y = 10.0f;
		m_Position.z = -20.0f;
		break;
	default:
		break;
	}
}

void Camera::Draw()
{
	//ビューマトリクス設定
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&m_ViewMatrix,
		DirectX::XMMatrixLookAtLH(XMLoadFloat3(&m_Position),
			XMLoadFloat3(&m_Target), XMLoadFloat3(&up))
	);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	//プロジェクションマトリクス設定
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMStoreFloat4x4(&projectionMatrix,
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60),
			(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f)
	);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

