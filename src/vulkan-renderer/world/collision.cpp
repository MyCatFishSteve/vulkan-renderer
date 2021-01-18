#include "inexor/vulkan-renderer/world/collision.hpp"

#include <glm/gtx/intersect.hpp>
#include <spdlog/spdlog.h>

#include <cassert>
#include <cmath>

namespace inexor::vulkan_renderer::world {

CollisionData::CollisionData(std::shared_ptr<Cube> cube) : m_cube(cube) {}

OctreeCollision::OctreeCollision(std::shared_ptr<Cube> world) : m_world(world) {}

bool OctreeCollision::ray_collides_with_octree(const glm::vec3 position, const glm::vec3 direction) const {
    const auto cube = m_world.lock();
    auto distance = 0.0f;
    const auto result =
        glm::intersectRaySphere(position, direction, cube->center(), std::pow(cube->size() / 2, 2), distance);
    return result;
}

bool OctreeCollision::ray_collides_with_box(const std::array<glm::vec3, 2> box_bounds, const glm::vec3 position,
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
    const auto cube = m_world.lock();

    if (cube->type() == Cube::Type::EMPTY) {
        return std::nullopt;
    }

    // First, check if ray collides with bounding sphere.
    if (!ray_collides_with_octree(position, direction)) {
        return std::nullopt;
    } else {
        if (ray_collides_with_box(cube->bounding_box(), position, direction)) {
            if (!cube->is_leaf()) {
                const auto &subcubes = cube->childs();
                for (std::int32_t i = 0; i < 8; i++) {
                    if (subcubes[i]->type() != Cube::Type::EMPTY) {
                        OctreeCollision subcollision(subcubes[i]);
                        if (subcollision.check_for_collision(position, direction)) {
                            spdlog::debug("Collided with subcube no {}.", i);
                        }
                    }
                }
            }
            return std::make_optional<CollisionData>(cube);
        } else {
            spdlog::trace("Camera ray collides with bounding sphere!");
        }

        // TODO: Check for octree collision!
        // TODO: For a simple approach, just check for collision with all 8 sub cubes.
    }

    return std::nullopt;
}

} // namespace inexor::vulkan_renderer::world
