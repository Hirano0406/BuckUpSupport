// GameÉVÅ[Éì [game.cpp]

#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "sky.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "Bullet.h"
#include "Explosion.h"
#include "box.h"
#include "cylinder.h"
#include "goal.h"
#include "result.h"
#include "transition.h"
#include "score.h"
#include "input.h"
#include "audio.h"
#include "tank.h"


#include "game.h"



void Game::Init()
{
	Bullet::Load();
	Explosion::Load();

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>();
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(-5.0f, 0.0f, 5.0f));
		box->SetScale(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(-11.0f, 0.0f, 5.0f));
		box->SetScale(DirectX::XMFLOAT3(3.0f, 2.0f, 3.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(-11.0f, 0.0f, 11.0f));
		box->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
	}
	{
		Cylinder* cylinder = AddGameObject<Cylinder>();
		cylinder->SetPosition(DirectX::XMFLOAT3(-11.0f, 0.0f, 20.0f));
		cylinder->SetScale(DirectX::XMFLOAT3(3.0f, 6.0f, 3.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f));
		box->SetScale(DirectX::XMFLOAT3(9.0f, 3.0f, 1.0f));
	}
	{
		Cylinder* cylinder = AddGameObject<Cylinder>();
		cylinder->SetPosition(DirectX::XMFLOAT3(11.0f, 0.0f, 20.0f));
		cylinder->SetScale(DirectX::XMFLOAT3(3.0f, 6.0f, 3.0f));
	}
	{
		Goal* goal = AddGameObject<Goal>();
		goal->SetPosition(DirectX::XMFLOAT3(11.0f, 9.0f, 20.0f));
		goal->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	}
	AddGameObject<Player>();

	AddGameObject<Enemy>()->SetPosition(
		DirectX::XMFLOAT3(0.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>()->SetPosition(
		DirectX::XMFLOAT3(5.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>()->SetPosition(
		DirectX::XMFLOAT3(10.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>()->SetPosition(
		DirectX::XMFLOAT3(15.0f, 0.0f, 5.0f));

	AddGameObject<Tank>()->SetPosition(DirectX::XMFLOAT3(20.0f, 0.0f, 5.0f));
	AddGameObject<Score>(4);
	m_BGM = AddGameObject<GameObject>(1)->AddComponent<Audio>();
	m_BGM->Load("asset/audio/bgm.wav");
	m_BGM->Play(true);
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();

}

void Game::Uninit()
{
	Explosion::Unload();
	Bullet::UnLoad();
}

void Game::Update()
{
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();
		if (goal == nullptr)
		{
			m_Goal = true;
			//Manager::SetScene<Result>();
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);

		}
	}
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Result>();
	}
}
