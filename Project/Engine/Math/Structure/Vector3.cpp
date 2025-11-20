#include "Vector3.h"

#include <algorithm>
#include <numbers>

// コンストラクタ
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

const Vector3 Vector3::ExprUnitXYZ = { 1.0f,1.0f,1.0f };
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
    
    float T = 0.0f;
    // ---------- Catmull‑Rom 由来の接線 ------------------------
    Vector3 m1 = (p2 - p0) * 0.5f * (1.0f - T);   // T=0 → 0.5,  T=1 → 0
    Vector3 m2 = (p3 - p1) * 0.5f * (1.0f - T);

    // ---------- Cubic Hermite 補間 ----------------------------
    float t2 = t * t;
    float t3 = t2 * t;

    float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h10 = t3 - 2.0f * t2 + t;
    float h01 = -2.0f * t3 + 3.0f * t2;
    float h11 = t3 - t2;

    return  h00 * p1 +              // 始点
        h10 * m1 +              // 始点接線
        h01 * p2 +              // 終点
        h11 * m2;               // 終点接線
}

Vector3 Vector3::CatmullRomPosition(const std::vector<Vector3>& points, float t) {
    // 2点だけなら線形補間
    if (points.size() == 2) {
        return points[0] + (points[1] - points[0]) * std::clamp(t, 0.0f, 1.0f);
    }

    // 3点だけならベジエ補間
    if (points.size() == 3) {
        float u = 1.0f - t;
        return  u * u * points[0] +
            2 * u * t * points[1] +
            t * t * points[2];
    }

    // 4点以上で曲線補間
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

Vector3 Vector3::DirectionToEuler(const Vector3& direction)
{
    Vector3 d = direction;
    float len2 = d.x*d.x + d.y*d.y + d.z*d.z;
    if (len2 < 1e-12f) return Vector3(0, 0, 0);
    float invLen = 1.0f / std::sqrt(len2);
    d.x *= invLen; d.y *= invLen; d.z *= invLen;

    float yaw = std::atan2(d.x, d.z);

    float xz = std::sqrt(d.x*d.x + d.z*d.z);
    float pitch = std::atan2(-d.y, std::max(xz, 1e-12f));

    float roll = 0.0f;

    return Vector3(pitch, yaw, roll);
}

Vector3 Vector3::TransformPoint(const Vector3& p, const Matrix4x4& m)
{
    float x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + 1.0f * m.m[3][0];
    float y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + 1.0f * m.m[3][1];
    float z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + 1.0f * m.m[3][2];
    // View/World はアフィンなので w=1 のまま返してOK
    return Vector3{ x, y, z };
}

Vector3 Vector3::TransformPointProjective(const Vector3& p, const Matrix4x4& m)
{
    float x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + 1.0f * m.m[3][0];
    float y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + 1.0f * m.m[3][1];
    float z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + 1.0f * m.m[3][2];
    float w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + 1.0f * m.m[3][3];
    if (w != 0.0f) { float invW = 1.0f / w; x *= invW; y *= invW; z *= invW; }
    return Vector3{ x, y, z };
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

bool Vector3::operator==(const Vector3& v)
{
    return (x == v.x && y == v.y && z == v.z);
}

// フレンド関数：スカラー倍の演算子オーバーロード
Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

float& Vector3::operator[](int i) {
    assert(i >= 0 && i < 3);
    return *(&x + i);
}

const float& Vector3::operator[](int i) const {
    assert(i >= 0 && i < 3);
    return *(&x + i);
}