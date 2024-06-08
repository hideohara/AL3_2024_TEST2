#pragma 

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"



class Player
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
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    // テクスチャハンドル
    uint32_t textureHandle_ = 0u;


    ViewProjection* viewProjection_ = nullptr;


    Vector3 velocity_ = {};

    // 加速
    static inline const float kAcceleration = 0.01f;
    //減速
    static inline const float kAttenuation = 0.02f;
    // 最大速度
    static inline const float kLimitRunSpeed = 1.0f;


    // 左右
    enum class LRDirection {
        kRight,
        kLeft,
    };
    LRDirection lrDirection_ = LRDirection::kRight;

    // 旋回開始時の角度
    float turnFirstRotationY_ = 0.0f;
    // 旋回タイマー
    float turnTimer_ = 0.0f;
    // 旋回時間<秒>
    static inline const float kTimeTurn = 0.3f;







};

