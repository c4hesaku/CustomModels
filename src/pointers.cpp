#include "pointers.hpp"

#include "GlobalNamespace/VRController.hpp"
#include "UnityEngine/Transform.hpp"
#include "VRUIControls/VRPointer.hpp"
#include "colors.hpp"
#include "main.hpp"
#include "metacore/shared/input.hpp"
#include "metacore/shared/unity.hpp"
#include "saber.hpp"

static std::set<std::string> const PointerHandleObjects = {"Normal", "Glowing", "FakeGlow0", "FakeGlow1"};

static bool hasMenuPointers = false;
static UnityEngine::GameObject* leftParent;
static UnityEngine::GameObject* rightParent;

static void SetHandlesEnabled(bool value) {
    for (auto& saber : {leftParent, rightParent}) {
        auto anchor = saber->transform->parent;
        for (auto& name : PointerHandleObjects) {
            if (auto child = anchor->Find(name))
                child->gameObject->active = value;
        }
    }
}

static void CreateMenuPointers() {
    auto input = MetaCore::Input::GetCurrentInputModule();
    if (!input) {
        logger.error("failed to find input module for menu pointers");
        return;
    }
    auto left = input->_vrPointer->_leftVRController;
    auto right = input->_vrPointer->_rightVRController;

    leftParent = UnityEngine::GameObject::New_ctor("CustomModelsLeftMenuPointer");
    rightParent = UnityEngine::GameObject::New_ctor("CustomModelsRightMenuPointer");

    leftParent->transform->SetParent(left->_viewAnchorTransform, false);
    rightParent->transform->SetParent(right->_viewAnchorTransform, false);

    CustomModels::InitSaber(leftParent->transform, true, GlobalNamespace::SaberType::SaberA);
    CustomModels::InitSaber(rightParent->transform, true, GlobalNamespace::SaberType::SaberB);

    hasMenuPointers = true;

    MetaCore::Engine::SetOnDestroy(leftParent, []() { hasMenuPointers = false; });
    MetaCore::Engine::SetOnDestroy(rightParent, []() { hasMenuPointers = false; });

    logger.debug("created menu pointers");
}

void CustomModels::EnableMenuPointers() {
    if (!hasMenuPointers)
        CreateMenuPointers();
    if (!hasMenuPointers)
        return;

    SetHandlesEnabled(false);
    leftParent->active = true;
    rightParent->active = true;
}

void CustomModels::DisableMenuPointers() {
    if (!hasMenuPointers)
        return;

    SetHandlesEnabled(true);
    leftParent->active = false;
    rightParent->active = false;
}

void CustomModels::DestroyMenuPointers() {
    if (!hasMenuPointers)
        return;

    SetHandlesEnabled(true);
    UnityEngine::Object::DestroyImmediate(leftParent);
    UnityEngine::Object::DestroyImmediate(rightParent);

    // OnDestroy isn't called if the object is inactive
    hasMenuPointers = false;

    logger.debug("destroyed menu pointers");
}

void CustomModels::UpdateMenuPointersColor() {
    if (!hasMenuPointers)
        return;

    UpdateSaberColor(leftParent->transform, true, GlobalNamespace::SaberType::SaberA);
    UpdateSaberColor(rightParent->transform, true, GlobalNamespace::SaberType::SaberB);
}
