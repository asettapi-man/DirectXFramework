struct vertexIn
{
    float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 col : COLOR0;
	float2 tx0 : TEXCOORD0;
};
 
struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
	float2 tx0 : TEXCOORD0;
};
 
cbuffer ConstantBuffer
{
	float4x4	WVP;		//World*View*Proj
	float4x4	World;		//ワールド行列
	float4		LightDir;	//平行光源の(逆)方向
	float4		LightCol;	//平行光源の色
	float4		LightAmb;	//環境光の色
    float4		CameraPos;	//カメラのワールド座標
    float4		Diffuse;	//拡散色
    float4		Ambient;	//環境色
    float4		Specular;	//鏡面反射色
    float4		TilingOffset; //テクスチャのタイリングとオフセット
}

vertexOut main(vertexIn IN )
{
	vertexOut OUT;
 
	//頂点座標をビューポート座標に変換
	OUT.pos = mul(IN.pos, WVP);

	//頂点カラー
	OUT.col = IN.col;

	//テクスチャ座標
    OUT.tx0 = IN.tx0 * TilingOffset.xy + TilingOffset.zw;

    return OUT;
}