#include "physics/mesh_shape_cache.h"

#include "Jolt/Core/Array.h"
#include "Jolt/Core/Reference.h"
#include "Jolt/Geometry/IndexedTriangle.h"
#include "Jolt/Math/Float3.h"
#include "Jolt/Physics/Collision/Shape/ConvexHullShape.h"
#include "Jolt/Physics/Collision/Shape/MeshShape.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include "asset/asset.h"
#include "asset/asset_manager.h"
#include "components/mesh_collider.h"
#include "core/application.h"
#include "renderer/mesh.h"

std::unordered_map<Core::AssetID, JPH::RefConst<JPH::Shape>> Physics::MeshShapeCache::s_concave_cache;
std::unordered_map<Core::AssetID, JPH::RefConst<JPH::Shape>> Physics::MeshShapeCache::s_convex_cache;

JPH::RefConst<JPH::Shape> Physics::MeshShapeCache::GetOrBuild(Core::AssetID mesh_id, Core::ColliderShapeType type)
{
    auto& cache = (type == Core::ColliderShapeType::ConvexMesh) ? s_convex_cache : s_concave_cache;

    if(auto it = cache.find(mesh_id); it != cache.end())
    {
        return it->second;
    }

    auto mesh_asset = Core::AssetManager::GetAsset<Core::MeshAsset>(mesh_id);

    if(!mesh_asset)
    {
        Core::LogMessageError("Mesh asset not found for collider mesh build !");
        return nullptr;
    }

    const Renderer::Mesh* mesh = mesh_asset->GetMesh();

    JPH::RefConst<JPH::Shape> shape;

    if(type == Core::ColliderShapeType::ConcaveMesh)
    {
        JPH::VertexList verts;
        verts.reserve(mesh->m_vertices.size());
        for(auto& v : mesh->m_vertices)
        {
            verts.emplace_back(v.position.x * 100.f, v.position.y * 100.f, v.position.z * 100.f); // Multiply by 100 because the mesh importer does so (and it doesn't bother the renderer then)
        }

        JPH::IndexedTriangleList tris;
        tris.reserve(mesh->m_indices.size() / 3);
        for(size_t i = 0; i + 2 < mesh->m_indices.size(); i+=3)
        {
            tris.emplace_back(mesh->m_indices[i], mesh->m_indices[i + 1], mesh->m_indices[i + 2]);
        }

        JPH::MeshShapeSettings settings(verts, tris);
        auto result = settings.Create();

        if(result.IsValid())
        {
            shape = result.Get();
        }

        else {
            Core::LogMessageError("Failed building concave mesh: " + std::string(result.GetError()));
        }
    }

    else {
        JPH::Array<JPH::Vec3> points;

        points.reserve(mesh->m_vertices.size());

        for(auto& v : mesh->m_vertices)
        {
            points.emplace_back(v.position.x * 100.f, v.position.y * 100.f, v.position.z * 100.f); // Same thing here
        }

        JPH::ConvexHullShapeSettings settings(points);

        auto result = settings.Create();

        if(result.IsValid())
        {
            shape = result.Get();
        }
        else
        {
            Core::LogMessageError("Failed building convex shape: " + std::string(result.GetError()));
        }
    }

    cache[mesh_id] = shape;
    return shape;
}