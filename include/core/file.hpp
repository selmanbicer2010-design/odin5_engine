#pragma once
#include "core/enum.hpp"
#include "core/utilities.hpp"
#include <concepts>
#include <string>
#include <vector>
#include <fstream>

namespace odin5{
namespace file{

    namespace detail{
        using file_path_t = const std::string&;
        template <typename t>
        concept is_file_read_t = std::same_as<t, std::vector<uint8_t>> or std::same_as<t, std::string>;
        inline void error_ptr(odin5::enm::error_t error_as, odin5::enm::error_ptr_t error_p) {
            if (error_p) {
                *error_p = error_as;
                odin5::util::error_if(static_cast<bool>(error_as), std::string{"file err, enm is " + std::to_string(error_as.value)}.c_str());
            }
        }
    }

    bool exists(odin5::file::detail::file_path_t path, odin5::enm::error_ptr_t error_p = nullptr);
    uint64_t size(odin5::file::detail::file_path_t path, odin5::enm::error_ptr_t error_p = nullptr);

    template <odin5::file::detail::is_file_read_t ret_t = std::vector<uint8_t>>
    ret_t read(odin5::file::detail::file_path_t path, odin5::enm::error_ptr_t error_p = nullptr) {
        odin5::file::detail::error_ptr(odin5::enm::err::NONE, error_p);
        if (!odin5::file::exists(path, error_p)) { if (*error_p != odin5::enm::err::FILE_OS_ERROR) { odin5::file::detail::error_ptr(odin5::enm::err::FILE_NOT_FOUND, error_p); }
            return {};
        }
        std::ifstream file;
        if constexpr (std::same_as<ret_t, std::vector<uint8_t>>) {
            file = std::ifstream{path, std::ios::binary};
        }
        else {
            file = std::ifstream{path};
        }

        if (!file.is_open()) {
            odin5::file::detail::error_ptr(odin5::enm::err::FILE_COULD_NOT_BE_OPENED, error_p);
            return {};
        }

        std::streamsize size = odin5::file::size(path);

        file.seekg(0, std::ios::beg);

        ret_t buffer;
        buffer.resize(size);

        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            odin5::file::detail::error_ptr(odin5::enm::err::FILE_COULD_NOT_BE_READ, error_p);
            return {};
        }

        return buffer;
    }
}
}
