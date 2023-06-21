#include "ScenePostProcess.h"
#include "Model.h"
#include "CameraBase.h"
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"

void ScenePostProcess::Init()
{
	// シェーダー読み込み
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),
		CreateObj<PixelShader>("PS_TexColor"),
		CreateObj<PixelShader>("PS_Monochrome"),
		CreateObj<PixelShader>("PS_Sepia"),
	};
	const char* filePath[] = {
		"Assets/Shader/VS_Object.cso",
		"Assets/Shader/PS_TexColor.cso",
		"Assets/Shader/PS_Monochrome.cso",
		"Assets/Shader/PS_Sepia.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);
	
	// パラメータ設定
	m_index = 0;
	m_isMulti = false;

	// モデル読み込み
	Model* pField = CreateObj<Model>("FieldModel");
	pField->Load("Assets/Model/field/field.fbx");
	// レンダーターゲット作成
	RenderTarget* pRTV = 
		CreateObj<RenderTarget>("PostProcessRTV");
	pRTV->Create(DXGI_FORMAT_R8G8B8A8_UNORM,
		SCREEN_WIDTH, SCREEN_HEIGHT);

}
void ScenePostProcess::Uninit()
{
}
void ScenePostProcess::Update(float tick)
{
	const int shaderNum = 3;
	if(IsKeyTrigger(VK_LEFT)){
		m_index -= 1;
		if (m_index < 0) {
			m_index = shaderNum - 1;
		}
	}
	if (IsKeyTrigger(VK_RIGHT))	{
		m_index += 1;
		if (m_index >= shaderNum) {
			m_index = 0;
		}
	}
}
void ScenePostProcess::Draw()
{
	// 描画情報
	Model* pModel = GetObj<Model>("Model");
	Model* pField = GetObj<Model>("FieldModel");
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	LightBase* pLight = GetObj<LightBase>("Light");
	RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
	RenderTarget* pPostProcessRTV =
		GetObj<RenderTarget>("PostProcessRTV");
	DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");

	// 各種シェーダー取得
	Shader* shader[] = {
		GetObj<Shader>("VS_Object"),
		GetObj<Shader>("PS_TexColor"),
		nullptr,	//	Spriteのデフォルトシェーダー
		GetObj<Shader>("PS_Monochrome"),
		GetObj<Shader>("PS_Sepia"),
	};

	// 変換行列設定
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();

	//	描画先(レンダーターゲット)の変更
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pPostProcessRTV->Clear(color);
	SetRenderTargets(1, &pPostProcessRTV, pDefDSV);

	//	今まで通りの描画
	shader[0]->WriteBuffer(0, mat);
	pModel->SetVertexShader(shader[0]);
	pModel->SetPixelShader(shader[1]);
	pModel->Draw();
	pField->SetVertexShader(shader[0]);
	pField->SetPixelShader(shader[1]);
	pField->Draw();

	//	元の描画に戻す
	SetRenderTargets(1, &pDefRTV, nullptr);

	//	レンダーターゲットに書き込まれたテクスチャを
	//	スプライトに貼り付けて表示
	
	//	3Dの表示から2Dの表示へ切り替え
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[1],
		DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2],
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixOrthographicLH(1, 1, 0.0f, 1.0f)
		));
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	//	レンダーターゲットを貼り付けて描画
	Sprite::SetPixelShader(shader[m_index + 2]);
	Sprite::SetTexture(pPostProcessRTV);
	Sprite::Draw();
	
	//	この後に、グリッド線の描画を行うので、3Dに戻す
	SetRenderTargets(1, &pDefRTV, pDefDSV);
}