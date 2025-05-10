#include "trail.hpp"

#include "GlobalNamespace/SaberMovementData.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/TimeHelper.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Texture.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/Vector2.hpp"
#include "defaults.hpp"
#include "main.hpp"
#include "metacore/shared/il2cpp.hpp"
#include "metacore/shared/operators.hpp"

DEFINE_TYPE(CustomModels, CustomSaberTrail);

static constexpr float StaticTrailLength = 2;

static ConstArray<UnityEngine::Vector3, 4> vertices;
static ConstArray<UnityEngine::Color, 4> colors;
static ConstArray triangles = {0, 3, 1, 0, 2, 3};
static ConstArray uvs = {UnityEngine::Vector2(1, 0), UnityEngine::Vector2(0, 0), UnityEngine::Vector2(1, 1), UnityEngine::Vector2(0, 1)};

void CustomModels::CustomSaberTrail::Init(UnityEngine::Material* material) {
    logger.info("custom trail init");
    _trailRenderer = GetTrailRenderer();
    _movementData = GlobalNamespace::SaberMovementData::New_ctor()->i___GlobalNamespace__IBladeMovementData();

    _samplingFrequency = 120;
    _granularity = 45;

    if (material) {
        // some trails have bad looking wrapping at the end due to imprecision and mip maps and stuff
        material->mainTexture->wrapMode = UnityEngine::TextureWrapMode::Clamp;
        _trailRenderer->_meshRenderer->material = material;
        _trailRenderer->_meshRenderer->material->color = _color;
    }
}

void CustomModels::CustomSaberTrail::SetStatic(bool value) {
    logger.debug("set static: {}", value);
    staticTrail = value;

    if (staticTrail && !staticMesh) {
        staticMesh = UnityEngine::Mesh::New_ctor();
        staticMesh->MarkDynamic();
    }

    _trailRenderer->_meshFilter->mesh = staticTrail ? staticMesh : _trailRenderer->_mesh.unsafePtr();

    RefreshTrail();
}

void CustomModels::CustomSaberTrail::RefreshTrail() {
    if (staticTrail) {
        logger.debug("refresh static trail");
        auto lengthVector = transform->right * (_trailDuration * StaticTrailLength);

        auto verticesArray = ArrayW<UnityEngine::Vector3>(vertices);
        verticesArray[0] = bottom->position;
        verticesArray[1] = top->position;
        verticesArray[2] = verticesArray[0] + lengthVector;
        verticesArray[3] = verticesArray[1] + lengthVector;

        auto colorsArray = ArrayW<UnityEngine::Color>(colors);
        for (int i = 0; i < colorsArray.size(); i++)
            colorsArray[i] = _color;

        staticMesh->vertices = vertices;
        staticMesh->colors = colors;
        staticMesh->uv = uvs;
        staticMesh->triangles = triangles;
        staticMesh->RecalculateBounds();
    } else if (_inited) {
        logger.debug("refresh normal trail");
        ResetTrailData();
        _trailRenderer->_trailWidth = (top->position - bottom->position).magnitude;
        _trailRenderer->_inverseTrailDuration = 1 / _trailDuration;
        _trailRenderer->_inverseSegmentDuration = _granularity / _trailDuration;
        _trailRenderer->_whiteSectionMaxDuration = _whiteSectionMaxDuration;
    }
}

bool CustomModels::CustomSaberTrail::PreLateUpdate() {
    if (staticTrail)
        return false;
    auto movementData = (GlobalNamespace::SaberMovementData*) _movementData;
    movementData->AddNewData(top->position, bottom->position, GlobalNamespace::TimeHelper::get_time());
    return true;
}

void CustomModels::CustomSaberTrail::OnDestroy() {
    GlobalNamespace::SaberTrail::OnDestroy();
    if (staticMesh)
        UnityEngine::Object::DestroyImmediate(staticMesh);
}
