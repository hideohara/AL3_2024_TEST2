#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete model_;
	delete modelBlock_;
	delete modelSkydome_;
	delete modelEnemy_;
	delete modelDeathParticles_;

	// クラスの解放
	delete player_;
	delete skydome_;
	delete debugCamera_;
	delete cameraController_;
	delete deathParticles_;

	//delete enemy_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();


	// マップチップフィールドの解放
	delete mapChipField_;



}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("test.png");

	// 3Dモデルの生成
	//model_ = Model::Create(); 
	model_ = Model::CreateFromOBJ("player");

	// ビュープロジェクションの初期化
	//viewProjection_.farZ = 1000;
	viewProjection_.Initialize();



	// ブロック3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(1000);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// スカイドームの生成
	skydome_ = new Skydome();
	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// マップ処理の生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// 自キャラの生成
	player_ = new Player();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 17);
	// 自キャラの初期化
	player_->Initialize(model_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// 追従カメラ
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = { 12.0f, 100 - 12.0f, 6.0f, 6.0f };
	cameraController_->SetMovableArea(cameraArea);

	// 敵
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	//enemy_ = new Enemy();
	//// 座標をマップチップ番号で指定
	//Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(11, 18);
	//// 自キャラの初期化
	//enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);



	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(11 + i * 3, 18 - i);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	// デスパーティクル
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}


// 更新
void GameScene::Update() {


	switch (phase_) {
	case Phase::kPlay:
		//ゲームプレイフェーズの処理
		// 自キャラの更新
		player_->Update();

		// スカイドームの更新
		skydome_->Update();

		// 追従カメラの更新
		cameraController_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		if (isDead_ == true) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			//自キャラの座標にデスパーティクルを発生、初期化
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		//デス演出フェーズの処理
		// スカイドームの更新
		skydome_->Update();

		// 追従カメラの更新
		cameraController_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// デスパーティクル
		if (deathParticles_) {
			deathParticles_->Update();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		
		viewProjection_.matView = debugCamera_->GetViewProjection().matView; //デバッグカメラのビュー行列;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection; // デバッグカメラのプロジェクション行列;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>


	// 自キャラの描画
	player_->Draw();

	//　スカイドームの描画
	skydome_->Draw();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// デスパーティクル
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks()
{
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}

}

void GameScene::CheckAllCollisions()
{
#pragma region 自キャラと敵キャラの当たり判定

	{
		// 判定対象1と2の座標
		AABB aabb1, aabb2;

		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				//player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				//enemy->OnCollision(player_);
				// デスフラグ
				isDead_ = true;
			}
		}
	}
#pragma endregion
}

void GameScene::ChangePhase()
{

	switch (phase_) {
	case Phase::kPlay:
		//ゲームプレイフェーズの処理
			break;
	case Phase::kDeath:
		//デス演出フェーズの処理
			break;
	}

}
