#include "defaults.hpp"

#include "BGLib/UnityExtension/AddressablesExtensions.hpp"
#include "GlobalNamespace/ColorNoteVisuals.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Transform.hpp"
#include "main.hpp"
#include "utils.hpp"

static GlobalNamespace::SaberTrailRenderer* trailRendererPrefab;
static UnityEngine::GameObject* defaultSaberPrefab;

static UnityEngine::GameObject* defaultNotePrefab;
static UnityEngine::GameObject* defaultBombPrefab;
static UnityEngine::GameObject* defaultChainHeadPrefab;
static UnityEngine::GameObject* defaultChainLinkPrefab;

static UnityEngine::GameObject* defaultWallPrefab;

static ConstString TrailRendererPath = "Assets/Prefabs/Effects/Sabers/SaberTrailRenderer.prefab";
static ConstString DefaultSaberPath = "Assets/Prefabs/Sabers/BasicSaberModel.prefab";

static ConstString DefaultNotePath = "Packages/com.beatgames.beatsaber.main.core/Prefabs/SongElements/Notes/NormalGameNote.prefab";
static ConstString DefaultBombPath = "Packages/com.beatgames.beatsaber.main.core/Prefabs/SongElements/Notes/BombNote.prefab";
static ConstString DefaultChainHeadPath =
    "Packages/com.beatgames.beatsaber.main.core/Prefabs/SongElements/Notes/BurstSliders/BurstSliderHeadNote.prefab";
static ConstString DefaultChainLinkPath = "Packages/com.beatgames.beatsaber.main.core/Prefabs/SongElements/Notes/BurstSliders/BurstSliderNote.prefab";

static ConstString DefaultWallPath = "Assets/Prefabs/SongElements/Obstacle.prefab";

static UnityEngine::GameObject* LoadAddressable(StringW path) {
    // lag spike is ok
    return BGLib::UnityExtension::AddressablesExtensions::LoadContent<UnityEngine::GameObject*>(static_cast<System::Object*>(path.convert()))
        ->get_Item(0);
}

void CustomModels::LoadDefaults() {
    logger.info("loading default prefabs...");
    trailRendererPrefab = LoadAddressable(TrailRendererPath)->GetComponent<GlobalNamespace::SaberTrailRenderer*>();
    defaultSaberPrefab = LoadAddressable(DefaultSaberPath);

    defaultNotePrefab = LoadAddressable(DefaultNotePath);
    defaultBombPrefab = LoadAddressable(DefaultBombPath);
    defaultChainHeadPrefab = LoadAddressable(DefaultChainHeadPath);
    defaultChainLinkPrefab = LoadAddressable(DefaultChainLinkPath);

    defaultWallPrefab = LoadAddressable(DefaultWallPath);
    logger.info("loaded default prefabs");
}

GlobalNamespace::SaberTrailRenderer* CustomModels::GetTrailRenderer() {
    return CustomModels::Instantiate(trailRendererPrefab);
}

UnityEngine::Material* CustomModels::GetDefaultTrailMaterial() {
    return trailRendererPrefab->_meshRenderer->material;
}

UnityEngine::GameObject* CustomModels::GetDefaultSaber() {
    auto saber = CustomModels::Instantiate(defaultSaberPrefab);
    for (auto component : saber->GetComponentsInChildren<GlobalNamespace::SetSaberGlowColor*>(true))
        component->enabled = false;
    for (auto component : saber->GetComponentsInChildren<GlobalNamespace::SetSaberFakeGlowColor*>(true))
        component->enabled = false;
    saber->GetComponent<GlobalNamespace::SaberTrail*>()->enabled = false;
    return saber;
}

static void InstantiateNote(UnityEngine::Transform* parent, std::string name, bool dot) {
    auto note = CustomModels::Instantiate(defaultNotePrefab, parent);
    note->name = name;
    auto visuals = note->GetComponent<GlobalNamespace::ColorNoteVisuals*>();
    visuals->showArrow = !dot;
    visuals->showCircle = dot;
}

static void InstantiateChain(UnityEngine::Transform* parent, std::string name, bool link) {
    auto chain = CustomModels::Instantiate(link ? defaultChainLinkPrefab : defaultChainHeadPrefab, parent);
    chain->name = name;
}

UnityEngine::GameObject* CustomModels::GetDefaultBomb() {
    auto bomb = CustomModels::Instantiate(defaultBombPrefab);
    bomb->name = "Bomb";
    return bomb;
}

UnityEngine::GameObject* CustomModels::GetDefaultNotes() {
    auto parent = UnityEngine::GameObject::New_ctor("NewDefaultNotes");

    auto notes = UnityEngine::GameObject::New_ctor("Notes")->transform;
    notes->SetParent(parent->transform, false);
    InstantiateNote(notes, "LeftArrow", false);
    InstantiateNote(notes, "RightArrow", false);
    InstantiateNote(notes, "LeftDot", true);
    InstantiateNote(notes, "RightDot", true);

    auto chains = UnityEngine::GameObject::New_ctor("Chains")->transform;
    chains->SetParent(parent->transform, false);
    InstantiateChain(chains, "LeftHead", false);
    InstantiateChain(chains, "RightHead", false);
    InstantiateChain(chains, "LeftLink", true);
    InstantiateChain(chains, "RightLink", true);

    GetDefaultBomb()->transform->SetParent(parent->transform, false);

    return parent;
}

UnityEngine::GameObject* CustomModels::GetDefaultWall() {
    return CustomModels::Instantiate(defaultWallPrefab);
}
