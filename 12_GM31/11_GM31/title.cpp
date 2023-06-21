// TitleÉVÅ[Éì [title.cpp]
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "shader.h"
#include "sprite.h"
#include "transition.h"


void Title::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);
	titleLogo->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "asset/texture/title.jpg");

	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();
}

void Title::Update()
{
	if (m_Transition->GetState() == Transition::State::Stop)
	{
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			//Manager::SetScene<Game>();
			m_Transition->FadeOut();
		}
	}
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}

}
