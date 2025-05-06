#pragma once

#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Renderer.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(CustomModels, ColorVisuals, UnityEngine::MonoBehaviour) {
    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, FetchMaterials);
    DECLARE_INSTANCE_METHOD(void, SetColor, UnityEngine::Color color, UnityEngine::Color otherColor);
    DECLARE_INSTANCE_METHOD(void, SetSidedColor, bool left);
    DECLARE_INSTANCE_METHOD(void, SetMenuColor, bool left);

    DECLARE_INSTANCE_FIELD(ArrayW<UnityEngine::Material*>, materials);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::MaterialPropertyBlockController*, propertyController);
    DECLARE_INSTANCE_FIELD(ArrayW<GlobalNamespace::SetSaberGlowColor*>, glows);
    DECLARE_INSTANCE_FIELD(ArrayW<GlobalNamespace::SetSaberFakeGlowColor*>, fakeGlows);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, initialized, false);
};

namespace CustomModels {
    extern GlobalNamespace::ColorScheme* colorScheme;

    UnityEngine::Color MenuLeftColor();
    UnityEngine::Color MenuRightColor();
    UnityEngine::Color MenuWallColor();
}
