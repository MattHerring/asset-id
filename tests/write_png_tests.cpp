#include <catch2/catch.hpp>
#include <png.h>

#include "write_png.h"

using namespace asset_id;

TEST_CASE("Destination must be a png")
{
    auto const asset_id = create_asset_id("7890");
    REQUIRE(asset_id);

    auto const digits = create_checked_asset_id(*asset_id);
    REQUIRE(digits);

    REQUIRE(!write_as_png(*digits, "/unkown_dir/7890.txt"));
}

TEST_CASE("libpng deallocators are null safe")
{
    // Another slightly odd test, but it is here as confirmation that you can call
    // deallocators on uninitialised pointers. I think this is necessary as the resource
    //  management pattern I am using for calling the png C-API can trigger deallocating 
    // on null pointers.
    png_struct* write_struct = nullptr;
    png_destroy_write_struct(&write_struct, nullptr);

    write_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    REQUIRE(write_struct);

    png_info* info_struct = nullptr;
    png_destroy_info_struct(nullptr, &info_struct);
    png_destroy_info_struct(write_struct, &info_struct);

    png_destroy_write_struct(&write_struct, nullptr);
}
