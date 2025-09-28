#include "write_png.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <png.h>

#include "image_line.h"

namespace
{
} // namespace

namespace asset_id
{
bool write_as_png(checked_asset_id_t const& asset_id, std::filesystem::path const& destination)
{
    if (destination.extension() != ".png")
    {
        std::cout << "Output must be a png file: " << destination.string() << "\n";
        return false;
    }

    auto const image_line_start_byte = 1U;

    auto pixels = create_image_line(asset_id, image_line_start_byte);
    if (!pixels)
    {
        std::cout << "Failed to create pixel row, skipping id.\n";
        return false;
    }

    bool result = false;
    FILE* outfile = nullptr;
    png_struct* write_struct = nullptr;
    png_info* info_struct = nullptr;

    do
    {
        outfile = fopen(destination.c_str(), "wb");
        if (!outfile)
        {
            std::cout << "Failed to access output file '" << destination.c_str()
                      << "', skipping id.\n";
            result = false;
            break;
        }

        write_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!write_struct)
        {
            std::cout << "Failed to create png write structure, skipping id.\n";
            result = false;
            break;
        }

        png_set_invert_mono(write_struct);

        info_struct = png_create_info_struct(write_struct);
        if (!info_struct)
        {
            std::cout << "Failed to create png info structure, skipping id.\n";
            result = false;
            break;
        }

        auto const colour_bit_depth = 1;
        auto const image_line_height_pixels = 1;

        png_init_io(write_struct, outfile);
        png_set_IHDR(
            write_struct,
            info_struct,
            image_line_width_pixels,
            image_line_height_pixels,
            colour_bit_depth,
            PNG_COLOR_TYPE_GRAY,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );

        png_write_info(write_struct, info_struct);

        auto* buf = pixels->data();
        png_write_image(write_struct, &buf);
        png_write_end(write_struct, info_struct);

    } while (false);

    png_destroy_info_struct(write_struct, &info_struct);
    png_destroy_write_struct(&write_struct, nullptr);

    auto const closeRC = fclose(outfile);
    if (closeRC != 0)
    {
        std::cout << "Failed to close file " << destination.filename() << "; ignoring.\n";
    }

    return true;
}

} // namespace asset_id