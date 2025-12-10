#pragma once

/// <summary>
/// パイプラインの種類を表す列挙体
/// </summary>
enum class PipelineType {
	kObject3d,
	kSprite,
	kLine3d,
	kParticle,
	kPrimitiveDrawr,
	kAnimation,
	kRenderTexture,
	kSkybox,
	kObjectOutLineMask,
	kAnimationOutLineMask,
	kObjectShadowMapDepth,
	kAnimationShadowMapDepth,
	kSpriteDissolve,
};

/// <summary>
/// ブレンドモードを表す列挙体
/// </summary>
enum class BlendMode {
	kBlendModeNone,			// ブレンド無し
	kBlendModeNormal,		// 通常αブレンド
	kBlendModeAdd,			// 加算ブレンド
	kBlendModeSubtract,		// 減算ブレンド
	kBlendModeMultily,		// 乗算ブレンド
	kCount
};