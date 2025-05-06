#pragma once

#include "UnityEngine/Transform.hpp"
#include "HMUI/IconSegmentedControl.hpp"

namespace CustomModels {
    void SetLayerRecursively(UnityEngine::Transform* object, int layer);

    void SetupIcons(HMUI::IconSegmentedControl* iconControl, int selected);
}
