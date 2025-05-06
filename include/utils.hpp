#pragma once

#include "HMUI/IconSegmentedControl.hpp"
#include "UnityEngine/Transform.hpp"

namespace CustomModels {
    void SetLayerRecursively(UnityEngine::Transform* object, int layer);

    void SetupIcons(HMUI::IconSegmentedControl* iconControl, int selected);

    template <class T>
    T Instantiate(T prefab) {
        return UnityEngine::Object::Instantiate(prefab, UnityEngine::Vector3::get_zero(), UnityEngine::Quaternion::get_identity());
    }

    template <class T>
    T Instantiate(T prefab, UnityEngine::Transform* parent) {
        return UnityEngine::Object::Instantiate(prefab, UnityEngine::Vector3::get_zero(), UnityEngine::Quaternion::get_identity(), parent);
    }
}
