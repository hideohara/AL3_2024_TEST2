#include "Player.h"
#include <cassert>
#include <numbers>
#include "Input.h"
#include <algorithm>

// 初期化
void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	// ローカル変数は関数が終わると消える
	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;


}

void Player::Update()
{

	// 移動入力

// 移動入力
// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				//現在のY回転の角度を旋回開始時の角度に代入
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				//旋回タイマーに旋回時間を代入
				turnTimer_ = kTimeTurn;
			}
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				//現在のY回転の角度を旋回開始時の角度に代入
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				//旋回タイマーに旋回時間を代入
				turnTimer_ = kTimeTurn;
			}
		}
		// 加速／減速
		velocity_ += acceleration;
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

	}
	else {
		// 非入力時は移動減衰をかける
		velocity_.x *= (1.0f - kAttenuation);
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	// 旋回制御
	if (turnTimer_ > 0.0f)
	{
		//旋回タイマーを1 / 60秒分カウントダウンする
		turnTimer_ -= 1.0f / 60.0f;
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f
		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y =
			EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列を更新
	worldTransform_.UpdateMatrix();


}

void Player::Draw()
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);


}
