#include "../../framework.h"
#include "../../Framework/environment.h"

//コンストラクタ
AABBCollider::AABBCollider()
{
	boundingBox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	boundingBox.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);

	debugLineColor = 0xff00ff00;	//デバッグ用の線の色（緑）
	isEnable = true;
}

void AABBCollider::initialize(const XMFLOAT3& center, const XMFLOAT3& extents)
{
	boundingBox.Center = center;
	boundingBox.Extents = extents;
}

//== 当たり判定の設定 ==
void AABBCollider::setBoundingBox(const BoundingBox& box)
{
	boundingBox.Center = box.Center;
	boundingBox.Extents = box.Extents;
}

void AABBCollider::setBoundingBoxCenter(const XMFLOAT3& center)
{
	boundingBox.Center = center;
}

void AABBCollider::setBoundingBoxCenter(float x, float y, float z)
{
	boundingBox.Center = XMFLOAT3(x, y, z);
}

void AABBCollider::setBoundingBoxExtents(const XMFLOAT3& extents)
{
	boundingBox.Extents = extents;
}

void AABBCollider::setBoundingBoxExtents(float x, float y, float z)
{
	boundingBox.Extents = XMFLOAT3(x, y, z);
}

//== AABB同士の当たり判定 ==
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

//== AABBColliderの当たり判定を線で可視化する ==
void AABBCollider::debugLineRenderer()
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
