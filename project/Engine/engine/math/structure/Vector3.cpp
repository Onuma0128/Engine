#include "Vector3.h"

#include <algorithm>
#include <numbers>

// コンストラクタ
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

const Vector3 Vector3::ExprUnitX = { 1.0f,0.0f,0.0f };
const Vector3 Vector3::ExprUnitY = { 0.0f,1.0f,0.0f };
const Vector3 Vector3::ExprUnitZ = { 0.0f,0.0f,1.0f };
const Vector3 Vector3::ExprZero = { 0.0f,0.0f,0.0f };

// 加算
Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// 減算
Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

// スカラー倍
Vector3 Vector3::Multiply(float scalar, const Vector3& v) {
    return { v.x * scalar, v.y * scalar, v.z * scalar };
}

float Vector3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

float Vector3::Distance(const Vector3& v1, const Vector3& v2)
{
    Vector3 result = v1 - v2;
    return std::sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
}

// 内積
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
    t = std::clamp(t, 0.0f, 1.0f);
    return Vector3(
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t,
        v1.z + (v2.z - v1.z) * t
    );
}

// 正規化
Vector3 Vector3::Normalize() const {
    float length = Length();
    assert(length != 0 && "正規化中にゼロ除算が発生しました。");
    return { x / length, y / length, z / length };
}

Vector3 Vector3::Transform(const Vector3& v, const Matrix4x4& matrix)
{
    Vector3 result{};
    result.x = v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = v.x * matrix.m[0][3] + v.y * matrix.m[1][3] + v.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;
    return result;
}

// 行列による変換（仮実装）
Vector3 Vector3::Transform(const Matrix4x4& matrix) const {
    Vector3 result{};
    result.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;
    return result;
}

Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix4x4& matrix)
{
    return {
            v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0],
            v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1],
            v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2]
    };
}

float Vector3::AxisComponent(const Vector3& v, int axis)
{
    switch (axis) {
    case 0:  return v.x;
    case 1:  return v.y;
    default: return v.z;
    }
}

Quaternion Vector3::FromEuler(const Vector3& eulerDeg)
{
    // ラジアンへ変換して半角を取る
    float kDeg2Rad = std::numbers::pi_v<float> / 180.0f;
    const float hx = eulerDeg.x * kDeg2Rad * 0.5f;   // pitch / 2
    const float hy = eulerDeg.y * kDeg2Rad * 0.5f;   // yaw   / 2
    const float hz = eulerDeg.z * kDeg2Rad * 0.5f;   // roll  / 2

    const float cx = std::cos(hx);
    const float sx = std::sin(hx);
    const float cy = std::cos(hy);
    const float sy = std::sin(hy);
    const float cz = std::cos(hz);
    const float sz = std::sin(hz);

    Quaternion q;
    // Y (yaw) → X (pitch) → Z (roll) の順
    q.w = cx * cy * cz + sx * sy * sz;
    q.x = sx * cy * cz + cx * sy * sz;
    q.y = cx * sy * cz - sx * cy * sz;
    q.z = cx * cy * sz - sx * sy * cz;
    return q;
}

Vector3 Vector3::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
    constexpr float kEps = 1e-5f;            // ← ゼロ除算防止
    auto horizDist = [](const Vector3& a, const Vector3& b) {
        float dx = a.x - b.x;
        float dz = a.z - b.z;                // 高さ(Y)は無視
        return std::sqrt(dx * dx + dz * dz);
        };

    //─── α = 0.5 なので √distance を足していく ─────────────
    float t0 = 0.0f;
    float t1 = t0 + std::pow(std::max(horizDist(p0, p1), kEps), 0.6f);
    float t2 = t1 + std::pow(std::max(horizDist(p1, p2), kEps), 0.6f);
    float t3 = t2 + std::pow(std::max(horizDist(p2, p3), kEps), 0.6f);

    //   区間 p1‑p2 の実パラメータ tt を求める
    float tt = std::lerp(t1, t2, std::clamp(t, 0.0f, 1.0f));

    //─── Shoemake (1985) の幾何的定義  ──────────────
    // まず A1,A2,A3
    float inv_t10 = 1.0f / std::max(t1 - t0, kEps);
    float inv_t21 = 1.0f / std::max(t2 - t1, kEps);
    float inv_t32 = 1.0f / std::max(t3 - t2, kEps);

    Vector3 A1 = Lerp(p0, p1, (tt - t0) * inv_t10);
    Vector3 A2 = Lerp(p1, p2, (tt - t1) * inv_t21);
    Vector3 A3 = Lerp(p2, p3, (tt - t2) * inv_t32);

    // 次に B1,B2
    float inv_t20 = 1.0f / std::max(t2 - t0, kEps);
    float inv_t31 = 1.0f / std::max(t3 - t1, kEps);

    Vector3 B1 = Lerp(A1, A2, (tt - t0) * inv_t20);
    Vector3 B2 = Lerp(A2, A3, (tt - t1) * inv_t31);

    // 最終点 C (= 補間結果)
    float inv_t21b = 1.0f / std::max(t2 - t1, kEps);
    Vector3 C = Lerp(B1, B2, (tt - t1) * inv_t21b);

    return C;
}

Vector3 Vector3::CatmullRomPosition(const std::vector<Vector3>& points, float t) {
    assert(points.size() >= 4 && "制御点が足りません");

    // 区間数は制御点の数 - 1
    size_t division = points.size() - 1;
    // 1区間の長さ (全体を 1.0 とした割合)
    float areaWidth = 1.0f / division;
    // 区間番号
    size_t index = static_cast<size_t>(t / areaWidth);
    // 区間番号が上限を超えないように収める
    index = std::clamp(index, size_t(0), division - 1);
    // 区間内の始点を 0.0f、終点を 1.0f とした時の現在位置
    float t_2 = (t - index * areaWidth) / areaWidth;
    t_2 = std::clamp(t_2, 0.0f, 1.0f);

    // 4点分のインデックス
    size_t index0 = (index == 0) ? 0 : index - 1;
    size_t index1 = index;
    size_t index2 = index + 1;
    size_t index3 = (index + 2 >= points.size()) ? points.size() - 1 : index + 2;

    // 4点の座標
    const Vector3& p0 = points[index0];
    const Vector3& p1 = points[index1];
    const Vector3& p2 = points[index2];
    const Vector3& p3 = points[index3];

    return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

// 単項演算子オーバーロード
Vector3 Vector3::operator+() const {
    return *this;
}

Vector3 Vector3::operator-() const {
    return { -x, -y, -z };
}

// 二項演算子オーバーロード
Vector3 Vector3::operator+(const Vector3& v) const {
    return { x + v.x, y + v.y, z + v.z };
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return { x - v.x, y - v.y, z - v.z };
}

Vector3 Vector3::operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

// 複合代入演算子オーバーロード
Vector3& Vector3::operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

// フレンド関数：スカラー倍の演算子オーバーロード
Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}