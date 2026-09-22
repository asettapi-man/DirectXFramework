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
	void initialize(const XMFLOAT3& center, const XMFLOAT3& extents);

	//== AABBの当たり判定の取得 ==
	inline BoundingBox& getBoundingBox() { return boundingBox; }	//AABBの当たり判定を取得

	//当たり判定の設定
	void setBoundingBox(const BoundingBox& box);
	void setBoundingBoxCenter(const XMFLOAT3& center);
	void setBoundingBoxCenter(float x, float y, float z);
	void setBoundingBoxExtents(const XMFLOAT3& extents);
	void setBoundingBoxExtents(float x, float y, float z);
	bool Intersects(const AABBCollider& other) const;	//AABB同士の当たり判定

	//有効・無効フラグの取得
	inline bool getEnable() { return isEnable; }

	//有効・無効フラグの設定
	inline void setEnable(bool enable) { isEnable = enable; }

	//デバッグ用
	void debugLineRenderer();
};
