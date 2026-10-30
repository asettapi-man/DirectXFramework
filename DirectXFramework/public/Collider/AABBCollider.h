#pragma once

class AABBCollider
{
private:
	BoundingBox boundingBox;	//AABBの当たり判定
	DWORD debugLineColor;	//デバッグ用の線の色

	bool isEnable;	//有効かどうかのフラグ

public:
	AABBCollider();	//コンストラクタ

	//初期化
	void Initialize(const XMFLOAT3& center, const XMFLOAT3& extents);

	//== AABBの当たり判定の取得 ==
	inline BoundingBox& GetBoundingBox() { return boundingBox; }	//AABBの当たり判定を取得

	//当たり判定の設定
	void SetBoundingBox(const BoundingBox& box);
	void SetBoundingBoxCenter(const XMFLOAT3& center);
	void SetBoundingBoxCenter(float x, float y, float z);
	void SetBoundingBoxExtents(const XMFLOAT3& extents);
	void SetBoundingBoxExtents(float x, float y, float z);
	bool Intersects(const AABBCollider& other) const;	//AABB同士の当たり判定

	//有効・無効フラグの取得
	inline bool GetEnable() { return isEnable; }

	//有効・無効フラグの設定
	inline void SetEnable(bool enable) { isEnable = enable; }

	//デバッグ用
	void DebugLineRenderer();
};
