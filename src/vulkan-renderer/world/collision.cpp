#include "inexor/vulkan-renderer/world/collision.hpp"

#include <glm/gtx/intersect.hpp>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <unordered_map>

namespace inexor::vulkan_renderer::world {

CollisionData::CollisionData(std::shared_ptr<Cube> cube) : m_cube(cube) {}

CollisionData::CollisionData() : m_cube(nullptr) {}

void CollisionData::reset() {
    m_cube = nullptr;
}

float CollisionData::squared_distance(glm::vec3 position) {
    return std::pow(m_cube->center().x - position.x, 2) + std::pow(m_cube->center().y - position.y, 2) +
           std::pow(m_cube->center().z - position.z, 2);
}

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
    std::int32_t sign[3]{inverse_dir.x < 0, inverse_dir.y < 0, inverse_dir.z < 0};

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

    return true;
}

void OctreeCollision::clear_hits() {
    for (auto hit : m_hits) {
        hit.reset();
    }
    m_hits_found = 0;
}

void OctreeCollision::add_hit(CollisionData collision) {
    m_hits[m_hits_found] = collision;
    m_hits_found++;
}

void OctreeCollision::sort_hits(const glm::vec3 position) {
    // TODO: Refactor! This implementation is stupid. There must be a better way of doing this!
    std::unordered_map<std::size_t, float> calculated_distances;
    for (std::size_t i = 0; i < m_hits_found; i++) {
        calculated_distances[i] = m_hits[i].squared_distance(position);
        spdlog::trace("Collision {}, squared distance {}", i, calculated_distances[i]);
    }

    // TODO: Search for the lowest squared distance.
    // TODO: Make sure the collision of nearest distance is in front of the camera.
    // TODO: Return sorted collisions.
}

std::optional<CollisionData> OctreeCollision::check_for_collision(const glm::vec3 position, const glm::vec3 direction) {
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
                /// TODO: Check which face and edge is selected.
                return std::make_optional<CollisionData>(m_cube);
            } else {
                clear_hits();
                /// Iterate through all sub cubes and check for collision.
                const auto &subcubes = m_cube->childs();
                for (std::int32_t i = 0; i < 8; i++) {
                    if (subcubes[i]->type() != Cube::Type::EMPTY) {
                        OctreeCollision subcollision(subcubes[i]);
                        const auto collision_result = subcollision.check_for_collision(position, direction);
                        if (collision_result) {
                            add_hit(collision_result.value());
                            // TODO: Check how many hits could be found at most (less than 8) and abort the loop.
                        }
                    }
                }

                if (m_hits_found > 1) {
                    sort_hits(position);
                }

                // TODO: Return collision.
            }
        }
    }

    return std::nullopt;
}

} // namespace inexor::vulkan_renderer::world
