#include "ScenePostProcess.h"
#include "Model.h"
#include "CameraBase.h"
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"

void ScenePostProcess::Init()
{
	// �V�F�[�_�[�ǂݍ���
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
	
	// �p�����[�^�ݒ�
	m_index = 0;
	m_isMulti = false;

	// ���f���ǂݍ���
	Model* pField = CreateObj<Model>("FieldModel");
	pField->Load("Assets/Model/field/field.fbx");
	// �����_�[�^�[�Q�b�g�쐬
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
	// �`����
	Model* pModel = GetObj<Model>("Model");
	Model* pField = GetObj<Model>("FieldModel");
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	LightBase* pLight = GetObj<LightBase>("Light");
	RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
	RenderTarget* pPostProcessRTV =
		GetObj<RenderTarget>("PostProcessRTV");
	DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");

	// �e��V�F�[�_�[�擾
	Shader* shader[] = {
		GetObj<Shader>("VS_Object"),
		GetObj<Shader>("PS_TexColor"),
		nullptr,	//	Sprite�̃f�t�H���g�V�F�[�_�[
		GetObj<Shader>("PS_Monochrome"),
		GetObj<Shader>("PS_Sepia"),
	};

	// �ϊ��s��ݒ�
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();

	//	�`���(�����_�[�^�[�Q�b�g)�̕ύX
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pPostProcessRTV->Clear(color);
	SetRenderTargets(1, &pPostProcessRTV, pDefDSV);

	//	���܂Œʂ�̕`��
	shader[0]->WriteBuffer(0, mat);
	pModel->SetVertexShader(shader[0]);
	pModel->SetPixelShader(shader[1]);
	pModel->Draw();
	pField->SetVertexShader(shader[0]);
	pField->SetPixelShader(shader[1]);
	pField->Draw();

	//	���̕`��ɖ߂�
	SetRenderTargets(1, &pDefRTV, nullptr);

	//	�����_�[�^�[�Q�b�g�ɏ������܂ꂽ�e�N�X�`����
	//	�X�v���C�g�ɓ\��t���ĕ\��
	
	//	3D�̕\������2D�̕\���֐؂�ւ�
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

	//	�����_�[�^�[�Q�b�g��\��t���ĕ`��
	Sprite::SetPixelShader(shader[m_index + 2]);
	Sprite::SetTexture(pPostProcessRTV);
	Sprite::Draw();
	
	//	���̌�ɁA�O���b�h���̕`����s���̂ŁA3D�ɖ߂�
	SetRenderTargets(1, &pDefRTV, pDefDSV);
}