#pragma once

#include <unordered_map>

#include "Jolt/Jolt.h"
#include "Jolt/Core/Reference.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include "asset/asset.h"
#include "components/mesh_collider.h"

namespace Physics
{

// TODO : Instead of a mesh cache, better build an asset type (like collidermeshasset that holds all the data)

class MeshShapeCache
{
public:
    static JPH::RefConst<JPH::Shape> GetOrBuild(Core::AssetID mesh_id, Core::ColliderShapeType type);
    static void Invalidate();

private:
    static std::unordered_map<Core::AssetID, JPH::RefConst<JPH::Shape>> s_concave_cache;
    static std::unordered_map<Core::AssetID, JPH::RefConst<JPH::Shape>> s_convex_cache;
};

}