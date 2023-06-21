// GameÉVÅ[Éì [game.h]
#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	class Audio*        m_BGM{};
	class Transition* m_Transition{};
	bool m_Goal = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
};
