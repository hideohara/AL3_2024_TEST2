#pragma once

#include "ViewProjection.h"
#include "MathUtilityForText.h"

// 前方宣言
class Player;

class CameraController
{

public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();


    void SetTarget(Player* target) { target_ = target; }


    void Reset();

    const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
    // ビュープロジェクション
    ViewProjection viewProjection_;


    Player* target_ = nullptr;

    // 追従対象とカメラの座標の差（オフセット）
    Vector3 targetOffset_ = { 0, 0, -15.0f };

};