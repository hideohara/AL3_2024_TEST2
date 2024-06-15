#pragma 

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"

class MapChipField;


class Player
{

public:
    struct CollisionMapInfo {
        bool ceiling = false;
        bool landing = false;
        bool hitWall = false;
        Vector3 move;
    };

    // 角
    enum Corner {
        kRightBottom,    // 右下
        kLeftBottom,     // 左下
        kRightTop,       // 右上
        kLeftTop,        // 左上

        kNumCorner       // 要素数
    };

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

    // 追加
    const WorldTransform& GetWorldTransform() const { return worldTransform_; }

    const Vector3& GetVelocity() const { return velocity_; }


    void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }




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


    // 重力加速度（下方向）
    static inline const float kGravityAcceleration = 0.05f;
    // 最大落下速度（下方向）
    static inline const float kLimitFallSpeed = 0.5f;
    // ジャンプ初速（上方向）
    static inline const float kJumpAcceleration = 0.5f;
    // 接地状態フラグ
    bool onGround_ = true;
    // 着地時の速度減衰率
    static inline const float kAttenuationLanding = 0.1f;

    // マップチップによるフィールド
    MapChipField* mapChipField_ = nullptr;


    // キャラクターの当たり判定サイズ
    static inline const float kWidth = 0.8f;
    static inline const float kHeight = 0.8f;

    // ①移動入力
    void InputMove();

    // ②マップ衝突判定
    void CheckMapCollision(CollisionMapInfo& info);
    void CheckMapCollisionUp(CollisionMapInfo& info);
    void CheckMapCollisionDown(CollisionMapInfo& info);

    // ③判定結果を反映して移動させる
    void CheckMapMove(CollisionMapInfo& info);

    // ④天井に接触している場合の処理
    void CheckMapCeiling(CollisionMapInfo& info);

    Vector3 CornerPosition(const Vector3& center, Corner corner);
    static inline const float kBlank = 0.2f;
    // ⑦旋回制御
    void AnimateTurn();

    //⑥接地状態の切り替え処理
    void CheckMapLanding(const CollisionMapInfo& info);

};

