#include "colors.hpp"

#include "GlobalNamespace/ColorSchemesSettings.hpp"
#include "GlobalNamespace/Parametric3SliceSpriteController.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "main.hpp"
#include "material.hpp"
#include "metacore/shared/game.hpp"
#include "metacore/shared/internals.hpp"
#include "metacore/shared/operators.hpp"

DEFINE_TYPE(CustomModels, ColorVisuals);

void CustomModels::ColorVisuals::FetchMaterials() {
    if (initialized)
        return;
    initialized = true;

    logger.debug("initialize colorable materials");

    std::vector<UnityEngine::Material*> materialsVector = {};
    std::vector<UnityW<UnityEngine::Renderer>> renderersVector = {};

    propertyController = GetComponentInParent<GlobalNamespace::MaterialPropertyBlockController*>();

    for (auto renderer : GetComponentsInChildren<UnityEngine::Renderer*>(true)) {
        bool addedRenderer = false;
        for (auto material : renderer->materials) {
            if (!material->HasProperty(Material::Color))
                continue;
            if (ShouldColor(material))
                materialsVector.push_back(material);
            else if (propertyController && !addedRenderer && ShouldColorReplaced(material)) {
                addedRenderer = true;
                renderersVector.push_back(renderer);
            }
        }
    }
    materials = ArrayW<UnityEngine::Material*>(materialsVector);

    glows = GetComponentsInChildren<GlobalNamespace::SetSaberGlowColor*>();
    fakeGlows = GetComponentsInChildren<GlobalNamespace::SetSaberFakeGlowColor*>();

    logger.debug(
        "found {} materials, {} default glows, and {} property controller",
        materials.size(),
        glows.size() + fakeGlows.size(),
        propertyController ? "a" : "no"
    );

    if (!propertyController)
        return;
    for (auto renderer : propertyController->_renderers)
        renderersVector.emplace_back(renderer);
    propertyController->_renderers = ArrayW<UnityW<UnityEngine::Renderer>>(renderersVector);
}

static void SetGlowColor(GlobalNamespace::SetSaberGlowColor* glow, UnityEngine::Color color) {
    auto propertyBlock = glow->_materialPropertyBlock;
    if (!propertyBlock)
        glow->_materialPropertyBlock = propertyBlock = UnityEngine::MaterialPropertyBlock::New_ctor();

    for (auto& pair : glow->_propertyTintColorPairs)
        propertyBlock->SetColor(pair->property, color * pair->tintColor);

    glow->_meshRenderer->SetPropertyBlock(propertyBlock);
}

static void SetGlowColor(GlobalNamespace::SetSaberFakeGlowColor* glow, UnityEngine::Color color) {
    glow->_parametric3SliceSprite->color = color;
    glow->_parametric3SliceSprite->Refresh();
}

void CustomModels::ColorVisuals::SetColor(UnityEngine::Color color, UnityEngine::Color otherColor) {
    if (!initialized)
        FetchMaterials();

    for (auto& material : materials) {
        material->SetColor(Material::Color, color);
        if (material->HasColor(Material::OtherColor))
            material->SetColor(Material::OtherColor, otherColor);
    }

    if (propertyController) {
        propertyController->materialPropertyBlock->SetColor(Material::Color, color);
        propertyController->ApplyChanges();
    }

    for (auto glow : glows)
        SetGlowColor(glow, color);
    for (auto glow : fakeGlows)
        SetGlowColor(glow, color);
}

void CustomModels::ColorVisuals::SetSidedColor(bool left) {
    auto scheme = ColorScheme();
    auto color = left ? scheme->_saberAColor : scheme->_saberBColor;
    auto otherColor = left ? scheme->_saberBColor : scheme->_saberAColor;

    SetColor(color, otherColor);
}

void CustomModels::ColorVisuals::SetMenuColor(bool left) {
    auto color = left ? MenuLeftColor() : MenuRightColor();
    auto otherColor = left ? MenuRightColor() : MenuLeftColor();

    SetColor(color, otherColor);
}

GlobalNamespace::ColorScheme* CustomModels::colorScheme = nullptr;

static inline GlobalNamespace::ColorScheme* SharedColors() {
    if (MetaCore::Internals::stateValid)
        return MetaCore::Internals::colors();
    return nullptr;
}

GlobalNamespace::ColorScheme* CustomModels::ColorScheme() {
    if (auto scheme = SharedColors())
        return scheme;
    return colorScheme;
}

static UnityEngine::Color const DefaultLeftColor = {0.78, 0.078, 0.078, 1};
static UnityEngine::Color const DefaultRightColor = {0.16, 0.56, 0.82, 1};
static UnityEngine::Color const DefaultWallColor = {0.78, 0.078, 0.078, 1};

static inline GlobalNamespace::ColorScheme* GetMenuColors() {
    if (auto scheme = SharedColors())
        return scheme;
    return MetaCore::Game::GetPlayerData()->_playerData->colorSchemesSettings->GetOverrideColorScheme();
}

UnityEngine::Color CustomModels::MenuLeftColor() {
    if (auto colors = GetMenuColors())
        return colors->_saberAColor;
    return DefaultLeftColor;
}

UnityEngine::Color CustomModels::MenuRightColor() {
    if (auto colors = GetMenuColors())
        return colors->_saberBColor;
    return DefaultRightColor;
}

UnityEngine::Color CustomModels::MenuWallColor() {
    if (auto colors = GetMenuColors())
        return colors->_obstaclesColor;
    return DefaultWallColor;
}
