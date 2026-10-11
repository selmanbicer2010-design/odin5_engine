#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
#include "core/math/spatial.hpp"
#include "core/enum.hpp"
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include "platform/graphics/graphicsapienum.hpp"

odin5::util::index_tuple<glm::vec3, glm::quat, glm::vec3> odin5::math::spatial::matrix_decompose(const glm::mat4& m) {
    odin5::util::index_tuple<glm::vec3, glm::quat, glm::vec3> ret;
    ret.get<0>() = m[3];
    ret.get<1>() = glm::quat_cast(m);
    ret.get<2>() = {glm::length(m[0]), glm::length(m[1]), glm::length(m[2])};
    return ret;
}

glm::vec3 odin5::math::spatial::transform_3d::x_normal() {
    return orientation * glm::vec3{1.f, 0.f, 0.f};
}

glm::vec3 odin5::math::spatial::transform_3d::y_normal() {
    return orientation * glm::vec3{0.f, 1.f, 0.f};
}

glm::vec3 odin5::math::spatial::transform_3d::z_normal() {
    return orientation * glm::vec3{0.f, 0.f, -1.f};
}

glm::mat4 odin5::math::spatial::camera_3d::view() {
    return glm::mat4_cast(glm::conjugate(transform.orientation)) * glm::translate(glm::mat4(1.0f), -transform.position);
}

glm::mat4 odin5::math::spatial::camera_3d::proj() {
    glm::mat4 proj = glm::perspective(fov, aspect, near_plane, far_plane);
    if constexpr (odin5::platform::graphics::enm::active_graphics_api == odin5::platform::graphics::enm::graphics_api_identifiers::vulkan) {
        proj[1][1] *= -1.f;
    }
    return proj;
}
