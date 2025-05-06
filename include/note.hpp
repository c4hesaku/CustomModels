#pragma once

#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/INoteControllerDidInitEvent.hpp"
#include "GlobalNamespace/NoteControllerBase.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Renderer.hpp"
#include "custom-types/shared/macros.hpp"
#include "json.hpp"
#include "loading.hpp"
#include "selection.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(CustomModels, NoteVisuals, UnityEngine::MonoBehaviour, GlobalNamespace::INoteControllerDidInitEvent*) {
    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, ShowObject, int);
    DECLARE_OVERRIDE_METHOD_MATCH(void, HandleNoteControllerDidInit, &GlobalNamespace::INoteControllerDidInitEvent::HandleNoteControllerDidInit, GlobalNamespace::NoteControllerBase*);

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);

    DECLARE_INSTANCE_FIELD(bool, isChain);
    DECLARE_INSTANCE_FIELD(ArrayW<UnityEngine::GameObject*>, objects);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, current, -1);
};

DECLARE_CLASS_CODEGEN(CustomModels, DebrisVisuals, UnityEngine::MonoBehaviour) {
    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, ShowDebris, GlobalNamespace::ColorType color, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);

    DECLARE_INSTANCE_METHOD(void, Awake);

    DECLARE_INSTANCE_FIELD(ArrayW<UnityEngine::GameObject*>, objects);

   public:
    std::map<UnityEngine::Renderer*, std::vector<UnityEngine::Material*>> leftMaterials;
    std::map<UnityEngine::Renderer*, std::vector<UnityEngine::Material*>> rightMaterials;
};

namespace CustomModels {
    struct Note : AssetInfo {
        NoteInfo info;

        void SetDefault() override;
        bool ParseInfo(std::string const& file) override;
        std::string ObjectName() override;
        void PostLoad() override;
    };

    enum class NoteType {
        Note,
        Bomb,
        ChainHead,
        ChainLink,
    };

    void InitNote(UnityEngine::GameObject* prefab, NoteType type, bool debris, bool mirror);

    UnityEngine::Transform* PreviewNotes(UnityEngine::Vector3 position, UnityEngine::Quaternion rotation);
    void UpdateNotesPreview(UnityEngine::Transform* preview);
}
