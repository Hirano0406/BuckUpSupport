// goalオブジェクト [goal.cpp]
#include "manager.h"
#include "modelRenderer.h"
#include "goal.h"
#include "shadow.h"
#include "shader.h"

void Goal::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/goal.obj");
	AddComponent<Shadow>()->SetSize(1.5f);
}
