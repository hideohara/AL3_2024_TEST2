#include "TitleScene.h"
#include "Input.h"
#include <numbers>

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	delete modelTitle_;
	delete modelPlayer_;
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	modelTitle_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");
	// ワールド変換の初期化
	worldTransformTitle_.Initialize();
	worldTransformPlayer_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	worldTransformTitle_.scale_ = { 2,2,2 };
	worldTransformTitle_.translation_ = { 0,8,0 };
	worldTransformPlayer_.translation_ = { 0,-8,0 };
	worldTransformPlayer_.scale_ = { 10,10,10 };
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	// 回転アニメーション
	playerTimer_ += 1.0f / 60.0f;
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float> + std::sin(std::numbers::pi_v<float> *2.0f * playerTimer_ / motionTime);

	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion


}
