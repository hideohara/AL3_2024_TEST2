#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// ローカル変数は関数が終わると消える
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update()
{
}

void Skydome::Draw()
{
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
