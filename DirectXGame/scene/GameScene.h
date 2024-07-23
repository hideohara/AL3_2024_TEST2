#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Skydome.h"
#include "MathUtilityForText.h"
#include <vector>
#include "MapChipField.h"
#include "DebugCamera.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	bool IsDead() const { return isDead_; }

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	//uint32_t textureHandle_ = 0;

	// プレイヤー3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;


	// 自キャラ
	Player* player_ = nullptr;

	// ブロック3Dモデル
	Model* modelBlock_ = nullptr;


	//std::vector<WorldTransform*> worldTransformBlocks_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// スカイドーム
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;


	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;


	void GenerateBlocks();

	CameraController* cameraController_ = nullptr;

	// 敵
	Model* modelEnemy_ = nullptr;
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;


	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// デスパーティクル
	DeathParticles* deathParticles_ = nullptr;
	Model* modelDeathParticles_ = nullptr;


	// ゲームのフェーズ（型）
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

	// ゲームの現在フェーズ（変数）
	Phase phase_;

	// フェーズの切り替え
	void ChangePhase();

	// デスフラグ
	bool isDead_ = false;

	// 終了フラグ
	bool finished_ = false;

};
