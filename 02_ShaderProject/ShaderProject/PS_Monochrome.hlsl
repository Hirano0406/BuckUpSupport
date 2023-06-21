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
	//	ポストプロセスで画像に校歌を加えるために、
	//	レンダーターゲットに書き込まれた色情報を取得
	color = tex.Sample(samp, pin.uv);

	//	モノクロ
	float mono = 0.0f;
#if 0	//	全体の平均からモノクロを計算
	mono += color.r;
	mono += color.b;
	mono += color.g;
	mono /= 3.0f;
#else
	//	人間の視覚特性を考慮したモノクロ
	//	※青色(輝度の低い色)の変化を感じずらい
	//	※緑色(輝度の高い色)の変化を感じやすい
	mono += color.r * 0.3f;
	mono += color.b * 0.6f;
	mono += color.g * 0.1f;
#endif
	color.rgb = mono;

	return color;
}