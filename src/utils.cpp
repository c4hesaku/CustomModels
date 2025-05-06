#include "utils.hpp"

#include "UnityEngine/GameObject.hpp"

void CustomModels::SetLayerRecursively(UnityEngine::Transform* object, int layer) {
    object->gameObject->layer = layer;
    int childCount = object->childCount;
    for (int i = 0; i < childCount; i++)
        SetLayerRecursively(object->GetChild(i), layer);
}

void CustomModels::SetupIcons(HMUI::IconSegmentedControl* iconControl, int selected) {
    iconControl->_hideCellBackground = false;
    iconControl->_overrideCellSize = true;
    iconControl->_iconSize = 4;
    iconControl->_padding = 0;
    iconControl->ReloadData();
    iconControl->SelectCellWithNumber(selected);
}
