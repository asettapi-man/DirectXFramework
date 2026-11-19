//--------------------------------------------------------------//
//	"AABBCollider.cpp"											//
//	AABBの当たり判定クラス										//
//																//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

#include "../../framework.h"
#include "../../Framework/environment.h"
#include "AABBCollider.h"

//コンストラクタ
AABBCollider::AABBCollider()
{
	boundingBox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	boundingBox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);

	debugLineColor = 0xff00ff00;	//デバッグ用の線の色（緑）
	isEnable = true;
}

void AABBCollider::Initialize(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	boundingBox.Center = center;
	boundingBox.Extents = extents;
}

/// <summary>
/// BoundingBox情報設定関数
/// </summary>
/// <param name="box">登録したいBoundingBox</param>
void AABBCollider::SetBoundingBox(const BoundingBox& box)
{
	boundingBox.Center = box.Center;
	boundingBox.Extents = box.Extents;
}

/// <summary>
/// BoundingBoxのCenter設定関数
/// </summary>
/// <param name="center">中央位置</param>
void AABBCollider::SetBoundingBoxCenter(const XMFLOAT3& center)
{
	boundingBox.Center = center;
}

/// <summary>
/// BoundingBoxのCenter設定関数
/// </summary>
/// <param name="x">x座標</param>
/// <param name="y">y座標</param>
/// <param name="z">z座標</param>
void AABBCollider::SetBoundingBoxCenter(float x, float y, float z)
{
	boundingBox.Center = XMFLOAT3(x, y, z);
}

/// <summary>
/// BoundingBoxのExtents設定関数
/// </summary>
/// <param name="extents">範囲（大きさ）</param>
void AABBCollider::SetBoundingBoxExtents(const XMFLOAT3& extents)
{
	boundingBox.Extents = extents;
}

/// <summary>
/// BoundingBoxのExtents設定関数
/// </summary>
/// <param name="x">x方向の大きさ</param>
/// <param name="y">y方向の大きさ</param>
/// <param name="z">z方向の大きさ</param>
void AABBCollider::SetBoundingBoxExtents(float x, float y, float z)
{
	boundingBox.Extents = XMFLOAT3(x, y, z);
}

/// <summary>
/// AABBCollider同士の当たり判定
/// </summary>
/// <param name="other">衝突対象</param>
/// <returns>衝突していればtrueが返る</returns>
bool AABBCollider::Intersects(const AABBCollider& other) const
{
	//無効な場合は当たり判定を行わない
	if (!isEnable || !other.isEnable)
	{
		return false;
	}

	//当たり判定
	if (boundingBox.Intersects(other.boundingBox))
	{
		return true;
	}

	return false;
}

/// <summary>
/// 当たり判定の線表示関数
/// </summary>
void AABBCollider::DebugLineRenderer()
{
	const XMFLOAT3& center = boundingBox.Center;
	const XMFLOAT3& extents = boundingBox.Extents;

	XMFLOAT3 v[8];	//8つの頂点座標を格納する配列

	//8つの頂点座標を計算
	//前面の4つの頂点
	v[0] = XMFLOAT3(center.x - extents.x, center.y - extents.y, center.z - extents.z);
	v[1] = XMFLOAT3(center.x + extents.x, center.y - extents.y, center.z - extents.z);
	v[2] = XMFLOAT3(center.x + extents.x, center.y + extents.y, center.z - extents.z);
	v[3] = XMFLOAT3(center.x - extents.x, center.y + extents.y, center.z - extents.z);

	//背面の4つの頂点
	v[4] = XMFLOAT3(center.x - extents.x, center.y - extents.y, center.z + extents.z);
	v[5] = XMFLOAT3(center.x + extents.x, center.y - extents.y, center.z + extents.z);
	v[6] = XMFLOAT3(center.x + extents.x, center.y + extents.y, center.z + extents.z);
	v[7] = XMFLOAT3(center.x - extents.x, center.y + extents.y, center.z + extents.z);

	//12本の線を描画
	//前面の4本線
	vnDebugDraw::Line(v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, debugLineColor);
	vnDebugDraw::Line(v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z, debugLineColor);
	vnDebugDraw::Line(v[2].x, v[2].y, v[2].z, v[3].x, v[3].y, v[3].z, debugLineColor);
	vnDebugDraw::Line(v[3].x, v[3].y, v[3].z, v[0].x, v[0].y, v[0].z, debugLineColor);
																	  
	//背面の4本線													 
	vnDebugDraw::Line(v[4].x, v[4].y, v[4].z, v[5].x, v[5].y, v[5].z, debugLineColor);
	vnDebugDraw::Line(v[5].x, v[5].y, v[5].z, v[6].x, v[6].y, v[6].z, debugLineColor);
	vnDebugDraw::Line(v[6].x, v[6].y, v[6].z, v[7].x, v[7].y, v[7].z, debugLineColor);
	vnDebugDraw::Line(v[7].x, v[7].y, v[7].z, v[4].x, v[4].y, v[4].z, debugLineColor);
																	
	//前面と背面を結ぶ4本線											
	vnDebugDraw::Line(v[0].x, v[0].y, v[0].z, v[4].x, v[4].y, v[4].z, debugLineColor);
	vnDebugDraw::Line(v[1].x, v[1].y, v[1].z, v[5].x, v[5].y, v[5].z, debugLineColor);
	vnDebugDraw::Line(v[2].x, v[2].y, v[2].z, v[6].x, v[6].y, v[6].z, debugLineColor);
	vnDebugDraw::Line(v[3].x, v[3].y, v[3].z, v[7].x, v[7].y, v[7].z, debugLineColor);
}
