#pragma once

#include "GlobalNamespace/FakeMirrorObjectsInstaller.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/MirroredBombNoteController.hpp"
#include "GlobalNamespace/MirroredGameNoteController.hpp"
#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/NoteDebrisPoolInstaller.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Transform.hpp"
#include "Zenject/DiContainer.hpp"
#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/objects/Beatmap.hpp"

DECLARE_CLASS_CODEGEN(CustomModels, CustomSaberAPI, GlobalNamespace::SaberModelController) {
    DECLARE_DEFAULT_CTOR();

    // lapiz API: needs to be named InitOverride, return value (if boolean) determines if SaberModelController::Init will also be run
    DECLARE_INSTANCE_METHOD(bool, InitOverride, UnityEngine::Transform* parent, GlobalNamespace::Saber* saber);
};

// extra registrations missing from lapiz
namespace Lapiz::Objects::Beatmap {
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(NoteDebrisHDRegistration, "_normalNoteDebrisHDPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(ChainHeadDebrisHDRegistration, "_burstSliderHeadNoteDebrisHDPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(ChainLinkDebrisHDRegistration, "_burstSliderElementNoteHDPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(NoteDebrisLWRegistration, "_normalNoteDebrisLWPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(ChainHeadDebrisLWRegistration, "_burstSliderHeadNoteDebrisLWPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(ChainLinkDebrisLWRegistration, "_burstSliderElementNoteLWPrefab", GlobalNamespace::NoteDebris*, GlobalNamespace::NoteDebrisPoolInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(MirrorNoteRegistration, "_mirroredGameNoteControllerPrefab", GlobalNamespace::MirroredGameNoteController*, GlobalNamespace::FakeMirrorObjectsInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(MirrorBombRegistration, "_mirroredBombNoteControllerPrefab", GlobalNamespace::MirroredBombNoteController*, GlobalNamespace::FakeMirrorObjectsInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(MirrorChainHeadRegistration, "_mirroredGameNoteControllerPrefab", GlobalNamespace::MirroredGameNoteController*, GlobalNamespace::FakeMirrorObjectsInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(MirrorChainLinkRegistration, "_mirroredGameNoteControllerPrefab", GlobalNamespace::MirroredGameNoteController*, GlobalNamespace::FakeMirrorObjectsInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(ObstacleRegistration, "_obstaclePrefab", GlobalNamespace::ObstacleController*, GlobalNamespace::BeatmapObjectsInstaller*);
    LAPIZ_REDECORATION_REGISTRATION_HELPER_DEFINITION(MirrorObstacleRegistration, "_mirroredObstacleControllerPrefab", GlobalNamespace::MirroredObstacleController*, GlobalNamespace::FakeMirrorObjectsInstaller*);
}

namespace CustomModels {
    constexpr int Priority = 300;

    extern GlobalNamespace::GameplayModifiers* modifiers;

    void Register(Zenject::DiContainer* container);
}
