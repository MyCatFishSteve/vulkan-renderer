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
    CollisionData();
    void reset();
    [[nodiscard]] float squared_distance(glm::vec3 position);
};

// TODO: Implement collision with multiple octree worlds.
class OctreeCollision {
private:
    // TODO: Maybe change this to a const reference?
    const std::shared_ptr<world::Cube> m_cube;

    /// Which cubes are hit?
    /// This serves as a std::vector, but avoids memory allocation.
    /// TODO: How many cubes can be hit as most? It is a number smaller than 8.
    std::array<CollisionData, 8> m_hits;
    std::uint32_t m_hits_found{0};

    // This serves as a replacement for std::vector.clear();
    void clear_hits();

    /// @brief Adds an octree collision to the collision list.
    /// @param collision The octree ray collision.
    void add_hit(CollisionData collision);

    /// @brief Sort octree collision hits by distance from position's point.
    /// @param position The origin point of the ray.
    void sort_hits(glm::vec3 position);

    /// @brief ``True`` if the ray build from the two vectors collides with the cube's bounding sphere.
    /// @param position The start position vector of the ray.
    /// @param direction The direction vector of the ray.
    /// @return ``True`` if the ray collides with the octree cube's bounding sphere.
    [[nodiscard]] bool ray_sphere_collision(glm::vec3 position, glm::vec3 direction) const;

    /// @brief ``True`` of the ray build from the two vectors collides with the cube's bounding box.
    /// @param box_bounds An array of two vectors which represent the edges of the bounding box.
    /// @param position The start position vector of the ray.
    /// @param direction The direction vector of the ray.
    /// @return ``True`` if the ray collides with the octree cube's bounding box.
    [[nodiscard]] bool ray_box_collision(std::array<glm::vec3, 2> box_bounds, glm::vec3 position,
                                         glm::vec3 direction) const;

public:
    /// @brief Default constructor.
    /// @param world The octree world.
    OctreeCollision(std::shared_ptr<world::Cube> world);

    /// @brief Checks for a collision between a camera ray and octree geometry.
    /// @param position The camera's position.
    /// @param direction The camera's view direction.
    /// @return
    std::optional<CollisionData> check_for_collision(glm::vec3 position, glm::vec3 direction);
};
} // namespace inexor::vulkan_renderer::world
