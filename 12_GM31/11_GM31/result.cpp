// Result�V�[�� [result.cpp]
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "result.h"
#include "shader.h"
#include "sprite.h"
#include "transition.h"

void Result::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);
	titleLogo->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "asset/texture/result.jpg");
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();
}

void Result::Update()
{
	if (m_Transition->GetState() == Transition::State::Stop)
	{
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			//Manager::SetScene<Title>();
			m_Transition->FadeOut();
		}
	}
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
	}
}
