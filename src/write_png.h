#pragma once

#include <filesystem>
#include <string_view>

#include "asset_id.h"

namespace asset_id
{
/**
* @brief 
* 
* @param asset_id     the checksum and asset id to render into a png file. 
* @param destination  the path of the file to create.
*
* NOTE: if `destination` is not accessible by the caller of this function then the 
* behaviour is undefined.
*
* @return true   if an instance of `image_line_t` representing `asset_id` could be 
*                created and saved as a png file.
* @return false  otherwise; this includes `destination` not having the extension `png`.
*/
bool write_as_png(checked_asset_id_t const& asset_id, std::filesystem::path const& destination);

} // namespace asset_id