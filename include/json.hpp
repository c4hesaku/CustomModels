#pragma once

#include "config-utils/shared/config-utils.hpp"

namespace CustomModels {
    DECLARE_JSON_STRUCT(TrailObject) {
        VALUE(int, trailId);
        VALUE(bool, isTop);
    };

    DECLARE_JSON_STRUCT(TrailInfo) {
        VALUE(int, trailId);
        VALUE(int, colorType);
        VALUE(ConfigUtils::Color, trailColor);
        VALUE(ConfigUtils::Color, multiplierColor);
        VALUE(int, length);
        VALUE(float, whiteStep);
    };

    DECLARE_JSON_STRUCT(SaberInfo) {
        VALUE_DEFAULT(bool, hasTrail, false);
        VALUE_DEFAULT(bool, keepFakeGlow, false);
        VALUE_DEFAULT(bool, isLegacy, false);
    };

    DECLARE_JSON_STRUCT(NoteInfo) {
        VALUE_DEFAULT(bool, hasDebris, false);
        VALUE_DEFAULT(bool, hasChainHeadDebris, false);
        VALUE_DEFAULT(bool, hasChainLinkDebris, false);
        VALUE_DEFAULT(bool, hasSlider, false);
        VALUE_DEFAULT(bool, hasBomb, false);
        VALUE_DEFAULT(bool, showArrows, false);
        VALUE_DEFAULT(bool, isMirrorable, false);
        VALUE_DEFAULT(bool, isLegacy, false);
    };

    DECLARE_JSON_STRUCT(WallInfo) {
        VALUE_DEFAULT(bool, replaceCoreMaterial, false);
        VALUE_DEFAULT(bool, replaceFrameMaterial, false);
        VALUE_DEFAULT(bool, replaceCoreMesh, false);
        VALUE_DEFAULT(bool, replaceFrameMesh, false);
        VALUE_DEFAULT(bool, disableCore, false);
        VALUE_DEFAULT(bool, disableFrame, false);
        VALUE_DEFAULT(bool, disableFakeGlow, false);
        VALUE_DEFAULT(bool, isMirrorable, false);
        VALUE_DEFAULT(bool, isLegacy, false);
    };

    DECLARE_JSON_STRUCT(Descriptor) {
        VALUE(std::string, author);
        VALUE(std::string, objectName);
        VALUE(std::string, description);
        VALUE_DEFAULT(std::string, coverImage, "");
    };

    DECLARE_JSON_STRUCT(Manifest) {
        VALUE(std::string, androidFileName);
        VALUE(Descriptor, descriptor);
        VALUE(UnparsedJSON, config);
    };

    DECLARE_JSON_STRUCT(LegacyDescriptor) {
        VALUE(std::string, authorName);
        VALUE(std::string, objectName);
        VALUE(std::string, description);
    };

    DECLARE_JSON_STRUCT(LegacyTrail) {
        VALUE(std::string, name);
        VALUE(int, colorType);
        VALUE(ConfigUtils::Color, trailColor);
        VALUE(ConfigUtils::Color, multiplierColor);
        VALUE(int, length);
        VALUE(float, whiteStep);
    };

    DECLARE_JSON_STRUCT(LegacySaberConfig) {
        VALUE(bool, enableFakeGlow);
        VALUE(bool, hasCustomTrails);
        VECTOR(LegacyTrail, leftTrails);
        VECTOR(LegacyTrail, rightTrails);
    };

    DECLARE_JSON_STRUCT(LegacyNoteConfig) {
        VALUE(bool, disableBaseGameArrows);
        VALUE(bool, hasBomb);
        VALUE(bool, hasDebris);
    };

    DECLARE_JSON_STRUCT(LegacyWallConfig) {
        VALUE(bool, replaceCoreMaterial);
        VALUE(bool, replaceCoreMesh);
        VALUE(bool, disableFrame);
        VALUE(bool, disableFakeGlow);
        VALUE(bool, replaceFrameMaterial);
        VALUE(bool, replaceFrameMesh);
        VALUE(bool, scoreSubmissionDisabled);
        VALUE(bool, moreThan1Core);
        VALUE(bool, moreThan1Frame);
    };
}
