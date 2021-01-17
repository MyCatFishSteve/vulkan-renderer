#pragma once

#include "inexor/vulkan-renderer/world/cube.hpp"

#include <glm/vec3.hpp>

#include <memory>
#include <optional>

namespace inexor::vulkan_renderer::world {

struct CollisionData {
    // The cube which collides with the camera ray.
    std::weak_ptr<Cube> cube;
};

// TODO: Implement collision with multiple octree worlds.
class OctreeCollision {
private:
    // TODO: Maybe change this to a const reference?
    const std::weak_ptr<world::Cube> m_world;

    /// @brief ``True`` if the ray build from the two vectors collides with the cube's bounding sphere.
    /// @param position The start position vector of the ray.
    /// @param direction The direction vector of the ray.
    /// @return ``True`` if the ray collides with the octree cube's bounding sphere.
    [[nodiscard]] bool ray_collides_with_octree(glm::vec3 position, glm::vec3 direction) const;

public:
    /// @brief Default constructor.
    /// @param world The octree world.
    OctreeCollision(std::shared_ptr<world::Cube> world);

    OctreeCollision(const OctreeCollision &) = delete;
    OctreeCollision(OctreeCollision &&) = delete;

    OctreeCollision &operator=(const OctreeCollision &) = delete;
    OctreeCollision &operator=(OctreeCollision &&) = delete;

    /// @brief
    /// @param position The camera's position.
    /// @param direction The camera's view direction.
    /// @return
    std::optional<CollisionData> check_for_collision(glm::vec3 position, glm::vec3 direction) const;
};
} // namespace inexor::vulkan_renderer::world
