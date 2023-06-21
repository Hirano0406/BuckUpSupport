// [manager.cpp]
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "audio.h"



Scene* Manager::m_Scene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();


	m_Scene = new Title();
	m_Scene->InitBase();
}

void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;


	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->DrawBase();

	Renderer::End();
}
