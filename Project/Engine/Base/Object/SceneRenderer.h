#pragma once
#include <list>
#include <memory>

#include "Object3d.h"
#include "Sprite.h"
#include "Line3d.h"
#include "PrimitiveDrawr.h"
#include "Animation.h"

// Forward declarations to ensure pointer types are available even if includes fail
class Object3d;
class Sprite;
class Line3d;
class PrimitiveDrawr;
class Animation;

/// <summary>
/// シーンの描画を管理するクラス
/// </summary>
class SceneRenderer
{
public:

	// 終了処理
	void Finalize();

	// 全ての描画
	void AllDraw();
	// 全ての描画(offscreenが終わってから描画)
	void OutAllDraw();

// Obejctをセットする
template<typename T>
void SetDrawList(T* object) { AddImpl(object); }
template<typename T>
void SetRemoveList(T* object) { RemoveImpl(object); }

// Add/Remove implementations for supported types
void AddImpl(Object3d* object) { objects_.push_back(object); }
void RemoveImpl(Object3d* object) { objects_.remove(object); }
void AddImpl(Sprite* sprite) { sprites_.push_back(sprite); }
void RemoveImpl(Sprite* sprite) { sprites_.remove(sprite); }
void AddImpl(Line3d* line) { lines_.push_back(line); }
void RemoveImpl(Line3d* line) { lines_.remove(line); }
void AddImpl(PrimitiveDrawr* primitive) { primitiveDrawrs_.push_back(primitive); }
void RemoveImpl(PrimitiveDrawr* primitive) { primitiveDrawrs_.remove(primitive); }
void AddImpl(Animation* animation) { animations_.push_back(animation); }
void RemoveImpl(Animation* animation) { animations_.remove(animation); }

private:

	/// ============================== 描画するObjects ============================== ///

	std::list<Object3d*>		objects_;
	std::list<Sprite*>			sprites_;
	std::list<Line3d*>			lines_;
	std::list<PrimitiveDrawr*>	primitiveDrawrs_;
	std::list<Animation*>		animations_;

};
