#include "inexor/vulkan-renderer/world/collision.hpp"

#include <glm/gtx/intersect.hpp>
#include <spdlog/spdlog.h>

#include <cassert>
#include <cmath>

namespace inexor::vulkan_renderer::world {

CollisionData::CollisionData(std::shared_ptr<Cube> cube) : m_cube(cube) {}

OctreeCollision::OctreeCollision(std::shared_ptr<Cube> cube) : m_cube(cube) {}

bool OctreeCollision::ray_sphere_collision(const glm::vec3 position, const glm::vec3 direction) const {
    auto distance = 0.0f;
    const auto result =
        glm::intersectRaySphere(position, direction, m_cube->center(), std::pow(m_cube->size() / 2, 2), distance);
    return result;
}

bool OctreeCollision::ray_box_collision(const std::array<glm::vec3, 2> box_bounds, const glm::vec3 position,
                                        const glm::vec3 direction) const {
    glm::vec3 inverse_dir{1 / direction.x, 1 / direction.y, 1 / direction.z};
    std::int32_t sign[3]{0, 0, 0};

    sign[0] = (inverse_dir.x < 0);
    sign[1] = (inverse_dir.y < 0);
    sign[2] = (inverse_dir.z < 0);

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (box_bounds[sign[0]].x - position.x) * inverse_dir.x;
    tmax = (box_bounds[1 - sign[0]].x - position.x) * inverse_dir.x;
    tymin = (box_bounds[sign[1]].y - position.y) * inverse_dir.y;
    tymax = (box_bounds[1 - sign[1]].y - position.y) * inverse_dir.y;

    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }
    if (tymin > tmin) {
        tmin = tymin;
    }
    if (tymax < tmax) {
        tmax = tymax;
    }

    tzmin = (box_bounds[sign[2]].z - position.z) * inverse_dir.z;
    tzmax = (box_bounds[1 - sign[2]].z - position.z) * inverse_dir.z;

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }

    return true;
}

std::optional<CollisionData> OctreeCollision::check_for_collision(const glm::vec3 position,
                                                                  const glm::vec3 direction) const {
    /// If the cube is empty, a collision with a ray is not possible,
    /// and there are no sub cubes to check for collision either.
    if (m_cube->type() == Cube::Type::EMPTY) {
        return std::nullopt;
    }

    /// First, check if ray collides with bounding sphere.
    /// This is much easier to calculate than collision with a bounding box.
    if (ray_sphere_collision(position, direction)) {
        /// Second, check if ray collides with bounding box.
        if (ray_box_collision(m_cube->bounding_box(), position, direction)) {
            if (m_cube->is_leaf()) {
                /// This is a leaf node, a collision has been found.
                /// TODO: Check which face is selected.
                /// TODO: Check which edge is selected.
                return std::make_optional<CollisionData>(m_cube);
            } else {
                /// Iterate through all sub cubes and check for collision.
                const auto &subcubes = m_cube->childs();
                for (std::int32_t i = 0; i < 8; i++) {
                    if (subcubes[i]->type() != Cube::Type::EMPTY) {
                        OctreeCollision subcollision(subcubes[i]);
                        if (subcollision.check_for_collision(position, direction)) {
                            spdlog::debug("Collided with subcube no {}.", i);
                        }
                    }
                }
            }
        }
    }

    return std::nullopt;
}

} // namespace inexor::vulkan_renderer::world
