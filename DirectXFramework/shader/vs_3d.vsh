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
    float3 nor : TEXCOORD1;	//法線ベクトル
    float3 eye : TEXCOORD2;	//視線ベクトル
};

cbuffer ConstantBuffer
{
	float4x4	WVP;			//World*View*Proj
	float4x4	World;			//ワールド行列
	float4		LightDir;		//平行光源の(逆)方向
	float4		LightCol;		//平行光源の色
	float4		LightAmb;		//環境光の色
    float4		CameraPos;		//カメラのワールド座標
    float4		Diffuse;		//拡散色
    float4		Ambient;		//環境色
    float4		Specular;		//鏡面反射色
    float4		TilingOffset;	//テクスチャのタイリングとオフセット
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	//頂点座標をビューポート座標に変換
	OUT.pos = mul(IN.pos, WVP);

	//法線ベクトルをワールド行列で変換
	float3 n = mul(float4(IN.nor.xyz, 0), World).xyz;
    OUT.nor = n;

	//頂点カラー
	OUT.col = IN.col;

	//ライティング
	OUT.col.xyz = IN.col.xyz * saturate(dot(normalize(n), LightDir.xyz)) * LightCol.xyz + LightAmb.xyz;
	
	//頂点座標（ローカル）をワールドマトリクスで変換
    float3 wPos = mul(IN.pos, World).xyz;
	
	//視線ベクトル
    OUT.eye = CameraPos.xyz - wPos;
	
	//マテリアルカラー
    OUT.col *= Diffuse;	//拡散色
    OUT.col.xyz += Ambient.xyz;	//環境色

	//テクスチャ座標
    OUT.tx0 = IN.tx0 * TilingOffset.xy + TilingOffset.zw;

	return OUT;
}