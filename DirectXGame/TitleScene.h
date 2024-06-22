#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"

class TitleScene
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;

	Model* modelTitle_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ワールド変換データ
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	DirectXCommon* dxCommon_ = nullptr;

	float playerTimer_ = 0;
	// アニメーションの周期となる時間[秒]
	static inline const float motionTime = 1.0f;
};

