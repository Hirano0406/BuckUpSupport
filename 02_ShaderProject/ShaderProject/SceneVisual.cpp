#include "SceneVisual.h"
#include "Model.h"
#include "CameraBase.h"
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"

void SceneVisual::Init()
{
	m_time = 0.0f;

	//	シェーダーの読み込み処理
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),
		CreateObj<PixelShader>("PS_TexColor"),
		CreateObj<VertexShader>("VS_WorldPosition"),
		CreateObj<PixelShader>("PS_DistanceFog"),
		CreateObj<PixelShader>("PS_HeightFog"),
		CreateObj<PixelShader>("PS_ScreenUV"),
		CreateObj<VertexShader>("VS_ScreenPosition"),
		CreateObj<PixelShader>("PS_PolarCoordinate"),
		CreateObj<PixelShader>("PS_Dissolve"),
		CreateObj<PixelShader>("PS_Moss"),
	};
	const char* file[] = {
		"Assets/Shader/VS_Object.cso",
		"Assets/Shader/PS_TexColor.cso",
		"Assets/Shader/VS_WorldPosition.cso",
		"Assets/Shader/PS_DistanceFog.cso",
		"Assets/Shader/PS_HeightFog.cso",
		"Assets/Shader/PS_ScreenUV.cso",
		"Assets/Shader/VS_ScreenPosition.cso",
		"Assets/Shader/PS_PolarCoordinate.cso",
		"Assets/Shader/PS_Dissolve.cso",
		"Assets/Shader/PS_Moss.cso",
	};

	int shaderNum = _countof(shader);
	for (int i = 0; i < shaderNum; ++i)
	{
		if (FAILED(shader[i]->Load(file[i])))
		{
			MessageBox(NULL, file[i], "Shader Error", MB_OK);
		}
	}
	Texture* pTex = CreateObj<Texture>("SpaceTex");
	pTex->Create("Assets/Texture/space.png");
	pTex = CreateObj<Texture>("DissolveTex");
	pTex->Create("Assets/Texture/dissolve.png");
}
void SceneVisual::Uninit()
{
}
void SceneVisual::Update(float tick)
{
	m_time += tick;
}
void SceneVisual::Draw()
{
	//	事前情報の取得
	Model* pModel = GetObj<Model>("Model");
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	LightBase* pLight = GetObj<LightBase>("Light");

	//	定数バッファで渡す行列の情報
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	//	定数バッファに渡すライトの情報
	DirectX::XMFLOAT3 lightDir =
		pLight->GetDirection();
	DirectX::XMFLOAT4 light[] = {
		pLight->GetDiffuse(),
		pLight->GetAmbient(),
		{lightDir.x, lightDir.y, lightDir.z, 0.0f}
	};

	//	定数バッファに渡すカメラの情報
	DirectX::XMFLOAT3 camPos = pCamera->GetPos();
	DirectX::XMFLOAT4 camera[] = {
		{camPos.x, camPos.y, camPos.z, 0.0f}
	};
	//	フォグの情報
	DirectX::XMFLOAT4 fog[] = {
		{0.8f, 0.9f, 1.0f, 1.0f},	//	フォグの色
		{3.0f, 3.0f, -0.1f, 0.1f},	//	距離(開始,適用)、高さ(開始,適用)
	};
	//	ゲージの情報
	DirectX::XMFLOAT4 polar[] = {
		//	外輪、内輪、ゲージ、未使用
		{0.5f, 0.1f, 1.0f - fmodf(m_time, 5.0f) / 5.0f, 0.0f}
	};
	//	ディゾルブの情報
	DirectX::XMFLOAT4 dissolve[] = {
		//	表示非表示の閾値、 変化範囲、 未使用x2
		{fmod(m_time, 5.0f) / 5.0f, 0.1f, 0.0f, 0.0f}
	};

	//	シェーダーの取得
	Shader* shader[] = {
		GetObj<Shader>("VS_Object"),
		GetObj<Shader>("PS_TexColor"),
		GetObj<Shader>("VS_WorldPosition"),
		GetObj<Shader>("PS_DistanceFog"),
		GetObj<Shader>("PS_HeightFog"),
		GetObj<Shader>("PS_ScreenUV"),
		GetObj<Shader>("VS_ScreenPosition"),
		GetObj<Shader>("PS_PolarCoordinate"),
		GetObj<Shader>("PS_Dissolve"),
		GetObj<Shader>("PS_Moss"),
	};
	int shaderPair[][2] = {
		{0, 1}, //	通常表示
		{2, 3}, //	距離フォグ
		{2, 4}, //	高さフォグ
		{6, 5}, //	スクリーン座標をUVとして利用
		{0, 7}, //	ゲージ(極座標)
		{0, 8},	//	ディゾルブ
		{0, 9},	//	苔
	};
	//	テクスチャの設定
	Texture* pTex = GetObj<Texture>("SpaceTex");
	shader[5]->SetTexture(1, pTex);
	pTex = GetObj<Texture>("DissolveTex");
	shader[8]->SetTexture(1, pTex);

	//	描画
	int drawNum = _countof(shaderPair);
	for (int i = 0; i < drawNum; ++i)
	{
		//	座標の更新
		DirectX::XMStoreFloat4x4(&mat[0],
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixTranslation(
				(drawNum - 1) * 0.5f - i, 0.0f, 0.0f
				)));

		//	定数バッファの更新
		shader[shaderPair[i][0]]->WriteBuffer(0, mat);
		shader[shaderPair[i][0]]->WriteBuffer(1, light);
		shader[shaderPair[i][1]]->WriteBuffer(0, light);
		shader[shaderPair[i][1]]->WriteBuffer(1, camera);
		shader[shaderPair[i][1]]->WriteBuffer(2, fog);
		if (i == 5)
			shader[shaderPair[i][1]]->WriteBuffer(1, dissolve);

		//	描画
		if (i == 4)
		{
			shader[shaderPair[i][1]]->WriteBuffer(0, polar);
			Sprite::SetPixelShader(shader[shaderPair[i][1]]);
			Sprite::SetWorld(mat[0]);
			Sprite::SetView(mat[1]);
			Sprite::SetProjection(mat[2]);
			Sprite::Draw();
		}
		else
		{
			pModel->SetVertexShader(shader[shaderPair[i][0]]);
			pModel->SetPixelShader(shader[shaderPair[i][1]]);
			pModel->Draw();
		}
	}
}