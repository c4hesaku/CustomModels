#pragma once

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"

namespace CustomModels {
    namespace Material {
        extern int CustomColors;
        extern int Glow;
        extern int Bloom;
        extern int Color;
        extern int OtherColor;
        extern int Alpha;
        extern int StencilRefID;
        extern int StencilComp;
        extern int StencilOp;
        extern int BlendSrcFactor;
        extern int BlendDstFactor;
        extern int BlendSrcFactorA;
        extern int BlendDstFactorA;
        extern int SlicePos;
        extern int CutPlane;
        extern int TransformOffset;
        extern int SizeParams;
    }

    bool ShouldColor(UnityEngine::Material* material);
    bool ShouldColorReplaced(UnityEngine::Material* material);

    void SetMirrorableProperties(UnityEngine::Material* material, bool mirror);

    void ReplaceMaterials(UnityEngine::GameObject* object);
    void ClearMaterialsCache();
}
