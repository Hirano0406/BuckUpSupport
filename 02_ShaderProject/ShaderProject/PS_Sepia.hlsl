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
	color = tex.Sample(samp, pin.uv);

	//	�܂����m�N���ɕϊ�
	color.rgb = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;

	//	�ΐF�ƐF�̐����������āA�S�̂ɐԂ݂��������摜�Ƃ���
	color.g *= 0.7f;
	color.b *= 0.4f;

	return color;
}