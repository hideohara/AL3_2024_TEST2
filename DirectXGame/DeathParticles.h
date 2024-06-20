#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"
#include <array>

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

};

