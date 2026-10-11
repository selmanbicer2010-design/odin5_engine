#pragma once
#include "core/utilities.hpp"

namespace odin5{
namespace platform{
namespace window{
namespace enm{

    struct window_api_identifier_t : odin5::util::type_safe_int32_wrapper<window_api_identifier_t> { using odin5::util::type_safe_int32_wrapper<window_api_identifier_t>::type_safe_int32_wrapper; };

    namespace window_api_identifiers {
        constexpr window_api_identifier_t unknown{0};
        constexpr window_api_identifier_t glfw{1};
        constexpr window_api_identifier_t sdl3{2};
    }

    constexpr window_api_identifier_t active_window_api = window_api_identifiers::glfw;

    struct fullscreen_mode_t : odin5::util::type_safe_int32_wrapper<fullscreen_mode_t> { using odin5::util::type_safe_int32_wrapper<fullscreen_mode_t>::type_safe_int32_wrapper; };

    namespace fullscreen_modes{
        constexpr fullscreen_mode_t free{0};
        constexpr fullscreen_mode_t exclusive{1};
        constexpr fullscreen_mode_t borderless{2};
    }

    struct enum_t : odin5::util::type_safe_int32_wrapper<enum_t> { using odin5::util::type_safe_int32_wrapper<enum_t>::type_safe_int32_wrapper; };

    constexpr enum_t etrue{1};
    constexpr enum_t efalse{0};

    constexpr enum_t focused = etrue;
    constexpr enum_t unfocused = efalse;

    constexpr enum_t iconified = etrue;
    constexpr enum_t uniconified = efalse;

    constexpr enum_t maximized = etrue;
    constexpr enum_t unmaximized = efalse;

    constexpr enum_t enter = etrue;
    constexpr enum_t exit = efalse;

    namespace input{
        struct action_t : odin5::util::type_safe_int32_wrapper<action_t> { using odin5::util::type_safe_int32_wrapper<action_t>::type_safe_int32_wrapper; };

        namespace action{
            constexpr action_t unknown{-1};
            constexpr action_t release{0};
            constexpr action_t press{1};
            constexpr action_t repeat{2};
        }

        struct key_t : odin5::util::type_safe_int32_wrapper<key_t> { using odin5::util::type_safe_int32_wrapper<key_t>::type_safe_int32_wrapper; };

        namespace keys{
            constexpr key_t unknown{256};

            constexpr key_t space{' '};
            constexpr key_t apostrophe{'\''};
            constexpr key_t comma{','};
            constexpr key_t minus{'-'};
            constexpr key_t period{'.'};
            constexpr key_t slash{'/'};

            constexpr key_t key_0{'0'};
            constexpr key_t key_1{'1'};
            constexpr key_t key_2{'2'};
            constexpr key_t key_3{'3'};
            constexpr key_t key_4{'4'};
            constexpr key_t key_5{'5'};
            constexpr key_t key_6{'6'};
            constexpr key_t key_7{'7'};
            constexpr key_t key_8{'8'};
            constexpr key_t key_9{'9'};

            constexpr key_t a{'a'};
            constexpr key_t b{'b'};
            constexpr key_t c{'c'};
            constexpr key_t d{'d'};
            constexpr key_t e{'e'};
            constexpr key_t f{'f'};
            constexpr key_t g{'g'};
            constexpr key_t h{'h'};
            constexpr key_t i{'i'};
            constexpr key_t j{'j'};
            constexpr key_t k{'k'};
            constexpr key_t l{'l'};
            constexpr key_t m{'m'};
            constexpr key_t n{'n'};
            constexpr key_t o{'o'};
            constexpr key_t p{'p'};
            constexpr key_t q{'q'};
            constexpr key_t r{'r'};
            constexpr key_t s{'s'};
            constexpr key_t t{'t'};
            constexpr key_t u{'u'};
            constexpr key_t v{'v'};
            constexpr key_t w{'w'};
            constexpr key_t x{'x'};
            constexpr key_t y{'y'};
            constexpr key_t z{'z'};

            constexpr key_t semicolon{';'};
            constexpr key_t equal{'='};
            constexpr key_t left_bracket{'['};
            constexpr key_t backslash{'\\'};
            constexpr key_t right_bracket{']'};
            constexpr key_t grave_accent{'`'};

            constexpr key_t world_1{257};
            constexpr key_t world_2{258};

            constexpr key_t escape{259};
            constexpr key_t enter{260};
            constexpr key_t tab{261};
            constexpr key_t backspace{262};
            constexpr key_t insert{263};
            constexpr key_t delete_key{264};

            constexpr key_t right{265};
            constexpr key_t left{266};
            constexpr key_t down{267};
            constexpr key_t up{268};

            constexpr key_t page_up{269};
            constexpr key_t page_down{270};
            constexpr key_t home{271};
            constexpr key_t end{272};

            constexpr key_t caps_lock{273};
            constexpr key_t scroll_lock{274};
            constexpr key_t num_lock{275};
            constexpr key_t print_screen{276};
            constexpr key_t pause{277};

            constexpr key_t f1{278};
            constexpr key_t f2{279};
            constexpr key_t f3{280};
            constexpr key_t f4{281};
            constexpr key_t f5{282};
            constexpr key_t f6{283};
            constexpr key_t f7{284};
            constexpr key_t f8{285};
            constexpr key_t f9{286};
            constexpr key_t f10{287};
            constexpr key_t f11{288};
            constexpr key_t f12{289};
            constexpr key_t f13{290};
            constexpr key_t f14{291};
            constexpr key_t f15{292};
            constexpr key_t f16{293};
            constexpr key_t f17{294};
            constexpr key_t f18{295};
            constexpr key_t f19{296};
            constexpr key_t f20{297};
            constexpr key_t f21{298};
            constexpr key_t f22{299};
            constexpr key_t f23{300};
            constexpr key_t f24{301};
            constexpr key_t f25{302};

            constexpr key_t kp_0{303};
            constexpr key_t kp_1{304};
            constexpr key_t kp_2{305};
            constexpr key_t kp_3{306};
            constexpr key_t kp_4{307};
            constexpr key_t kp_5{308};
            constexpr key_t kp_6{309};
            constexpr key_t kp_7{310};
            constexpr key_t kp_8{311};
            constexpr key_t kp_9{312};

            constexpr key_t kp_decimal{313};
            constexpr key_t kp_divide{314};
            constexpr key_t kp_multiply{315};
            constexpr key_t kp_subtract{316};
            constexpr key_t kp_add{317};
            constexpr key_t kp_enter{318};
            constexpr key_t kp_equal{319};

            constexpr key_t left_shift{320};
            constexpr key_t left_control{321};
            constexpr key_t left_alt{322};
            constexpr key_t left_super{323};

            constexpr key_t right_shift{324};
            constexpr key_t right_control{325};
            constexpr key_t right_alt{326};
            constexpr key_t right_super{327};

            constexpr key_t menu{328};
        };

        struct mods_t : odin5::util::type_safe_int32_wrapper<mods_t> { using odin5::util::type_safe_int32_wrapper<mods_t>::type_safe_int32_wrapper; };

        namespace mods{
            constexpr mods_t shift{0b1 << 1};
            constexpr mods_t control{0b1 << 2};
            constexpr mods_t alt{0b1 << 3};
            constexpr mods_t super{0b1 << 4};
            constexpr mods_t caps_lock{0b1 << 5};
            constexpr mods_t num_lock{0b1 << 6};
        }

        struct mouse_button_t : odin5::util::type_safe_int32_wrapper<mouse_button_t> { using odin5::util::type_safe_int32_wrapper<mouse_button_t>::type_safe_int32_wrapper; };

        namespace mouse_button{
            constexpr mouse_button_t unknown{-1};
            constexpr mouse_button_t e1{0};
            constexpr mouse_button_t e2{1};
            constexpr mouse_button_t e3{2};
            constexpr mouse_button_t e4{3};
            constexpr mouse_button_t e5{4};
        }

    }
}
}
}
}
