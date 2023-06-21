struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : C0LOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//	�|�X�g�v���Z�X�ŉ摜�ɍZ�̂������邽�߂ɁA
	//	�����_�[�^�[�Q�b�g�ɏ������܂ꂽ�F�����擾
	color = tex.Sample(samp, pin.uv);

	//	���m�N��
	float mono = 0.0f;
#if 0	//	�S�̂̕��ς��烂�m�N�����v�Z
	mono += color.r;
	mono += color.b;
	mono += color.g;
	mono /= 3.0f;
#else
	//	�l�Ԃ̎��o�������l���������m�N��
	//	���F(�P�x�̒Ⴂ�F)�̕ω����������炢
	//	���ΐF(�P�x�̍����F)�̕ω��������₷��
	mono += color.r * 0.3f;
	mono += color.b * 0.6f;
	mono += color.g * 0.1f;
#endif
	color.rgb = mono;

	return color;
}