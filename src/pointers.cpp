#include "pointers.hpp"

#include "UnityEngine/Transform.hpp"
#include "VRUIControls/VRPointer.hpp"
#include "colors.hpp"
#include "main.hpp"
#include "metacore/shared/input.hpp"
#include "metacore/shared/unity.hpp"
#include "saber.hpp"

static bool hasMenuPointers = false;
static UnityEngine::GameObject* leftParent;
static UnityEngine::GameObject* rightParent;

void CustomModels::EnableMenuPointers() {
    if (hasMenuPointers) {
        leftParent->active = true;
        rightParent->active = true;
        return;
    }

    auto input = MetaCore::Input::GetCurrentInputModule();
    if (!input) {
        logger.error("failed to find input module for menu pointers");
        return;
    }
    auto left = input->_vrPointer->_leftVRController;
    auto right = input->_vrPointer->_rightVRController;

    leftParent = UnityEngine::GameObject::New_ctor("CustomModelsLeftMenuPointer");
    rightParent = UnityEngine::GameObject::New_ctor("CustomModelsRightMenuPointer");

    leftParent->transform->SetParent(left->transform, false);
    rightParent->transform->SetParent(right->transform, false);

    InitSaber(leftParent->transform, true, GlobalNamespace::SaberType::SaberA);
    InitSaber(rightParent->transform, true, GlobalNamespace::SaberType::SaberB);

    hasMenuPointers = true;

    MetaCore::Engine::SetOnDestroy(leftParent, []() { hasMenuPointers = false; });
    MetaCore::Engine::SetOnDestroy(rightParent, []() { hasMenuPointers = false; });

    logger.debug("created menu pointers");
}

void CustomModels::DisableMenuPointers() {
    if (!hasMenuPointers)
        return;

    leftParent->active = false;
    rightParent->active = false;
}

void CustomModels::DestroyMenuPointers() {
    if (!hasMenuPointers)
        return;

    UnityEngine::Object::DestroyImmediate(leftParent);
    UnityEngine::Object::DestroyImmediate(rightParent);

    logger.debug("destroyed menu pointers");
}

void CustomModels::UpdateMenuPointersColor() {
    if (!hasMenuPointers)
        return;

    UpdateSaberColor(leftParent->transform, true, GlobalNamespace::SaberType::SaberA);
    UpdateSaberColor(rightParent->transform, true, GlobalNamespace::SaberType::SaberB);
}
