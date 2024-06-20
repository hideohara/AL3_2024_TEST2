#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"
#include <array>
#include <numbers>

class DeathParticles
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model"></param>
    /// <param name="textureHandle"></param>
    /// <param name="viewProjection"></param>
    void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:

    // パーティクルの個数
    static inline const uint32_t kNumParticles = 8;
    // ワールド変換データ
    std::array<WorldTransform, kNumParticles> worldTransforms_;

    // モデル
    Model* model_ = nullptr;

    ViewProjection* viewProjection_ = nullptr;


    // 存続時間（消滅までの時間）<秒>
    static inline const float kDuration = 1;
    // 移動の速さ
    static inline const float kSpeed = 0.1f;
    // 分割した1個分の角度
    static inline const float kAngleUnit = std::numbers::pi_v<float> / kNumParticles;

    // 終了フラグ
    bool isFinished_ = false;
    // 経過時間カウント
    float counter_ = 0.0f;

    // 色変更オブジェクト
    ObjectColor objectColor_;
    // 色の数値
    Vector4 color_;


};

