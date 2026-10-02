#include "core/file.hpp"
#include "core/enum.hpp"
#include <filesystem>
#include <system_error>

bool odin5::file::exists(odin5::file::detail::file_path_t path, odin5::enm::error_ptr_t error_p) {
    odin5::file::detail::error_ptr(odin5::enm::err::NONE, error_p);
    std::error_code ec;
    bool result = std::filesystem::exists(path, ec);
    if (ec) {
        odin5::file::detail::error_ptr(odin5::enm::err::FILE_OS_ERROR, error_p);
    }
    return result;
}

uint64_t odin5::file::size(odin5::file::detail::file_path_t path, odin5::enm::error_ptr_t error_p) {
    odin5::file::detail::error_ptr(odin5::enm::err::NONE, error_p);
    if (!odin5::file::exists(path, error_p)) { if (*error_p != odin5::enm::err::FILE_OS_ERROR) { odin5::file::detail::error_ptr(odin5::enm::err::FILE_NOT_FOUND, error_p); }
        return 0;
    }
    std::error_code ec;
    uint64_t size = std::filesystem::file_size(path, ec);
    if (ec) {
        odin5::file::detail::error_ptr(odin5::enm::err::FILE_OS_ERROR, error_p);
    }
    return size;
}
