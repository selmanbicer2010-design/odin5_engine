#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "core/utilities.hpp"

namespace odin5{
namespace math{
namespace spatial{
    struct vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    odin5::util::index_tuple<glm::vec3, glm::quat, glm::vec3> matrix_decompose(const glm::mat4& m);

    struct transform_3d {
        glm::vec3 position {0.f, 0.f, 0.f};
        glm::quat orientation {glm::identity<glm::quat>()};
        glm::vec3 scale {1.f, 1.f, 1.f};

        glm::vec3 x_normal();
        glm::vec3 y_normal();
        glm::vec3 z_normal();
    };

    struct camera_3d {
        transform_3d transform{};
        float fov {glm::pi<double>() * 0.5};
        float aspect {1.f};
        float near_plane {0.1f};
        float far_plane {1000.f};
        glm::mat4 view();
        glm::mat4 proj();

    };

}
}
}
