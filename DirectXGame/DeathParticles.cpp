#include "DeathParticles.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// ローカル変数は関数が終わると消える
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update()
{
	// 行列を更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw()
{
	// 3Dモデルを描画
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}
