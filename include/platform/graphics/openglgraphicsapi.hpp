#pragma once
#include "platform/graphics/igraphicsapi.hpp"

namespace odin5{
namespace platform{
namespace graphics{
namespace opengl{

    struct opengl_state {
    public:

    };

    class graphics_api_spec {
    private:
        opengl_state opengl_{};

    public:
        graphics_api_spec(odin5::platform::graphics::graphics_create_info gci);

        ~graphics_api_spec();
    };

    //using graphics_api = odin5::platform::graphics::graphics_api<graphics_api_spec>;
}
}
}
}
