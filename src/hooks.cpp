#include "hooks.hpp"

#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/NoteDebrisSpawner.hpp"
#include "GlobalNamespace/ObstacleMaterialSetter.hpp"
#include "Zenject/DiContainer.hpp"
#include "colors.hpp"
#include "config.hpp"
#include "metacore/shared/operators.hpp"
#include "note.hpp"
#include "registration.hpp"
#include "trail.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

// use the PC material to fix trails cutting into each other
MAKE_AUTO_HOOK_MATCH(ConditionalMaterialSwitcher_Awake, &ConditionalMaterialSwitcher::Awake, void, ConditionalMaterialSwitcher* self) {
    if (!getConfig().Enabled.GetValue() || self->name != "Trail(Clone)")
        ConditionalMaterialSwitcher_Awake(self);
}

// don't run the base class Awake on our saber trail
MAKE_AUTO_HOOK_MATCH(SaberTrail_Awake, &SaberTrail::Awake, void, SaberTrail* self) {
    if (!il2cpp_utils::try_cast<CustomModels::CustomSaberTrail>(self).has_value())
        SaberTrail_Awake(self);
}

// call PreLateUpdate to add movement data for our saber trail
MAKE_AUTO_HOOK_MATCH(SaberTrail_LateUpdate, &SaberTrail::LateUpdate, void, SaberTrail* self) {
    if (auto custom = il2cpp_utils::try_cast<CustomModels::CustomSaberTrail>(self)) {
        if (!(*custom)->PreLateUpdate())
            return;
    }
    SaberTrail_LateUpdate(self);
}

static void Scale(UnityEngine::Component* object, float size) {
    auto transform = object->transform;
    transform->localScale = transform->localScale * size;
}

// apply note size
MAKE_AUTO_HOOK_MATCH(
    NoteControllerInit,
    &NoteController::Init,
    void,
    NoteController* self,
    NoteData* noteData,
    ByRef<NoteSpawnData> noteSpawnData,
    float endRotation,
    float uniformScale,
    bool rotatesTowardsPlayer,
    bool useRandomRotation
) {
    float size = 1;
    if (getConfig().Enabled.GetValue())
        size = getConfig().NotesSettings().Size();

    NoteControllerInit(self, noteData, noteSpawnData, endRotation, uniformScale * size, rotatesTowardsPlayer, useRandomRotation);

    if (size == 1)
        return;
    float inverse = 1 / size;

    if (auto note = il2cpp_utils::try_cast<GameNoteController>(self).value_or(nullptr)) {
        for (auto hitbox : note->_bigCuttableBySaberList)
            Scale(hitbox, inverse);
        for (auto hitbox : note->_smallCuttableBySaberList)
            Scale(hitbox, inverse);
    } else if (auto bomb = il2cpp_utils::try_cast<BombNoteController>(self).value_or(nullptr))
        Scale(bomb->_cuttableBySaber, inverse);
}

// initialize custom debris
static NoteDebris* lastDebris0;
static NoteDebris* lastDebris1;

MAKE_AUTO_HOOK_MATCH(
    NoteDebrisSpawner_SpawnNoteDebris,
    &NoteDebrisSpawner::SpawnNoteDebris,
    void,
    NoteDebrisSpawner* self,
    NoteData::GameplayType noteGameplayType,
    ByRef<NoteDebris*> debris0,
    ByRef<NoteDebris*> debris1
) {
    NoteDebrisSpawner_SpawnNoteDebris(self, noteGameplayType, debris0, debris1);
    lastDebris0 = *debris0;
    lastDebris1 = *debris1;
}

MAKE_AUTO_HOOK_MATCH(
    NoteDebrisSpawner_SpawnDebris,
    &NoteDebrisSpawner::SpawnDebris,
    void,
    NoteDebrisSpawner* self,
    NoteData_GameplayType noteGameplayType,
    Vector3 cutPoint,
    Vector3 cutNormal,
    float saberSpeed,
    Vector3 saberDir,
    Vector3 notePos,
    Quaternion noteRotation,
    Vector3 noteScale,
    ColorType colorType,
    float timeToNextColorNote,
    Vector3 moveVec
) {
    NoteDebrisSpawner_SpawnDebris(
        self, noteGameplayType, cutPoint, cutNormal, saberSpeed, saberDir, notePos, noteRotation, noteScale, colorType, timeToNextColorNote, moveVec
    );
    if (!lastDebris0 || !lastDebris1)
        return;
    if (auto visuals = lastDebris0->GetComponentInChildren<CustomModels::DebrisVisuals*>())
        visuals->ShowDebris(colorType, cutPoint, -cutNormal);
    if (auto visuals = lastDebris1->GetComponentInChildren<CustomModels::DebrisVisuals*>())
        visuals->ShowDebris(colorType, cutPoint, cutNormal);
}

// selectively disable material setting for obstacle core
MAKE_AUTO_HOOK_MATCH(
    ObstacleMaterialSetter_SetCoreMaterial,
    &ObstacleMaterialSetter::SetCoreMaterial,
    void,
    ObstacleMaterialSetter* self,
    UnityEngine::Renderer* renderer,
    BeatSaber::Settings::QualitySettings::ObstacleQuality obstacleQuality,
    bool screenDisplacementEffects
) {
    if (self->_lwCoreMaterial && self->_texturedCoreMaterial && self->_hwCoreMaterial)
        ObstacleMaterialSetter_SetCoreMaterial(self, renderer, obstacleQuality, screenDisplacementEffects);
}

// get color manager
MAKE_AUTO_HOOK_MATCH(
    GameplayCoreSceneSetupData_ctor,
    &GameplayCoreSceneSetupData::_ctor,
    void,
    GameplayCoreSceneSetupData* self,
    ByRef<GlobalNamespace::BeatmapKey> p0,
    GlobalNamespace::BeatmapLevel* p1,
    GlobalNamespace::GameplayModifiers* modifiers,
    GlobalNamespace::PlayerSpecificSettings* p3,
    GlobalNamespace::PracticeSettings* p4,
    bool p5,
    GlobalNamespace::EnvironmentInfoSO* p6,
    GlobalNamespace::EnvironmentInfoSO* p7,
    GlobalNamespace::ColorScheme* colorScheme,
    GlobalNamespace::SettingsManager* p9,
    GlobalNamespace::AudioClipAsyncLoader* p10,
    GlobalNamespace::BeatmapDataLoader* p11,
    GlobalNamespace::BeatmapLevelsEntitlementModel* p12,
    bool p13,
    bool p14,
    GlobalNamespace::EnvironmentsListModel* p15,
    System::Nullable_1<GlobalNamespace::RecordingToolManager::SetupData> p16
) {
    GameplayCoreSceneSetupData_ctor(self, p0, p1, modifiers, p3, p4, p5, p6, p7, colorScheme, p9, p10, p11, p12, p13, p14, p15, p16);

    CustomModels::colorScheme = colorScheme;
    CustomModels::modifiers = modifiers;
}
