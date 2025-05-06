#pragma once

#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"

namespace CustomModels {
    void LoadDefaults();

    GlobalNamespace::SaberTrailRenderer* GetTrailRenderer();
    UnityEngine::Material* GetDefaultTrailMaterial();
    UnityEngine::GameObject* GetDefaultSaber();
    UnityEngine::GameObject* GetDefaultBomb();
    UnityEngine::GameObject* GetDefaultNotes();
    UnityEngine::GameObject* GetDefaultWall();
}
