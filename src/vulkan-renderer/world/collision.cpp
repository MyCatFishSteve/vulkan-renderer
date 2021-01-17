#include "inexor/vulkan-renderer/world/collision.hpp"

#include <glm/gtx/intersect.hpp>
#include <spdlog/spdlog.h>

#include <cassert>
#include <cmath>

namespace inexor::vulkan_renderer::world {

OctreeCollision::OctreeCollision(std::shared_ptr<Cube> world) : m_world(world) {}

bool OctreeCollision::ray_collides_with_octree(const glm::vec3 position, const glm::vec3 direction) const {
    const auto cube = m_world.lock();
    auto distance = 0.0f;
    const auto result =
        glm::intersectRaySphere(position, direction, cube->center(), std::pow(cube->size() / 2, 2), distance);
    return result;
}

std::optional<CollisionData> OctreeCollision::check_for_collision(const glm::vec3 position,
                                                                  const glm::vec3 direction) const {
    // First, check if ray collides with bounding sphere.
    if (!ray_collides_with_octree(position, direction)) {
        return std::nullopt;
    } else {
        spdlog::trace("Camera ray collides with bounding sphere!");
        // TODO: Check for octree collision!
    }

    return std::nullopt;
}

} // namespace inexor::vulkan_renderer::world
