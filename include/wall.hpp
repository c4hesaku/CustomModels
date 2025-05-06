#pragma once

#include "selection.hpp"

namespace CustomModels {
    struct Wall : AssetInfo {
        WallInfo info;

        void SetDefault() override;
        bool ParseInfo(std::string const& file) override;
        std::string ObjectName() override;
        void PostLoad() override;
    };

    void InitWall(UnityEngine::GameObject* prefab, bool mirror);

    UnityEngine::Transform* PreviewWalls(UnityEngine::Vector3 position, UnityEngine::Quaternion rotation);
    void UpdateWallsPreview(UnityEngine::Transform* preview);
}
