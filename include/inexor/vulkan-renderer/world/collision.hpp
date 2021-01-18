#pragma once

#include "inexor/vulkan-renderer/world/cube.hpp"

#include <glm/vec3.hpp>

#include <array>
#include <memory>
#include <optional>

namespace inexor::vulkan_renderer::world {

class CollisionData {
private:
    // The cube which collides with the camera ray.
    std::shared_ptr<Cube> m_cube;
    // TODO: Add info about which face and which edge.

public:
    CollisionData(std::shared_ptr<Cube> cube);
    CollisionData(const CollisionData &) = delete;
    CollisionData(CollisionData &&) = delete;

    CollisionData &operator=(const CollisionData &) = delete;
    CollisionData &operator=(CollisionData &&) = delete;
};

// TODO: Implement collision with multiple octree worlds.
class OctreeCollision {
private:
    // TODO: Maybe change this to a const reference?
    const std::shared_ptr<world::Cube> m_cube;

    /// @brief ``True`` if the ray build from the two vectors collides with the cube's bounding sphere.
    /// @param position The start position vector of the ray.
    /// @param direction The direction vector of the ray.
    /// @return ``True`` if the ray collides with the octree cube's bounding sphere.
    [[nodiscard]] bool ray_sphere_collision(glm::vec3 position, glm::vec3 direction) const;

    /// @brief ``True`` of tje ray build from the two vectors collides with the cube's bounding box.
    /// @param position The start position vector of the ray.
    /// @param direction The direction vector of the ray.
    /// @return ``True`` if the ray collides with the octree cube's bounding box.
    [[nodiscard]] bool ray_box_collision(std::array<glm::vec3, 2> box_bounds, glm::vec3 position,
                                         glm::vec3 direction) const;

public:
    /// @brief Default constructor.
    /// @param world The octree world.
    OctreeCollision(std::shared_ptr<world::Cube> world);

    OctreeCollision(const OctreeCollision &) = delete;
    OctreeCollision(OctreeCollision &&) = delete;

    OctreeCollision &operator=(const OctreeCollision &) = delete;
    OctreeCollision &operator=(OctreeCollision &&) = delete;

    /// @brief Checks for a collision between a camera ray and octree geometry.
    /// @param position The camera's position.
    /// @param direction The camera's view direction.
    /// @return
    std::optional<CollisionData> check_for_collision(glm::vec3 position, glm::vec3 direction) const;
};
} // namespace inexor::vulkan_renderer::world
