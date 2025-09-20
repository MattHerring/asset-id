#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "asset_id.h"
#include "write_png.h"

using namespace asset_id;

/**
 * @brief A linux specific helper function that flags whether the calling process 
 * can reach a particular path in a given access mode.
 * 
 * @param path     the path to consider.
 * @param as_mode  the access mode necessary.

 * @return true   if the specified path can be access in this mode.
 * @return false  otherwise.
 */
bool is_accessible(std::filesystem::path const& path, int as_mode)
{
    return (access(path.string().c_str(), as_mode) == 0);
}

namespace
{
void usage(void)
{
    std::cout << "Creates display pngs for specified list of asset ids.\n";
    std::cout << "Usage:\n 'asset_id' : displays this usage message.\n 'asset_id <INPUT_FILE> "
                 "<OUTPUT_DIR>' where:\n";
    std::cout << "\t <INPUT_FILE> is a path to a text file containing a list of 4 digit "
                 "asset ids, one per line. \n"
                 "\t <OUTPUT_DIR> is a path to a directory that "
                 "will hold the generated png files.\n";
    std::cout << "If a png file cannot be created for a given input row then this id will be "
                 "logged as a failure.\n";
    std::cout << "Caveats:\n"
                 "\t<INPUT_FILE> must exist and be a regular file.\n"
                 "\t<INPUT_FILE> must contain one id per line; each id must be at most 4 digits "
                 "long with no other characters present.\n"
                 "\t<OUTPUT_DIR> must exist and be writeable by the caller.\n";
    std::cout << "\tThe output files are written to <OUTPUT_DIR>; any existing files may be "
                 "overwritten, but will not be deleted.";

    std::cout << "\n";
}
} // namespace

int main(int argc, char* argv[])
{
    if ((1 != argc) && (3 != argc))
    {
        std::cout << "Unsupported number of arguments: " << argc << "\n";
        usage();
        return EXIT_FAILURE;
    }

    if (1 == argc)
    {
        usage();
        return EXIT_SUCCESS;
    }

    auto const input_file = std::filesystem::path{argv[1]};
    if (!is_accessible(input_file, R_OK))
    {
        std::cout << "ERROR: Input path " << input_file.string() << " is inaccessible.\n";
        return EXIT_FAILURE;
    }

    auto const output_dir = std::filesystem::path{argv[2]};
    if (!is_accessible(output_dir, W_OK))
    {
        std::cout << "ERROR: Output path " << output_dir.string() << " is inaccessible.\n";
        return EXIT_FAILURE;
    }

    auto input = std::ifstream(input_file);
    if (!input.is_open())
    {
        std::cout << "ERROR: Cannot open input file " << input_file.string() << " .\n";
        return EXIT_FAILURE;
    }

    std::string id_string;
    std::vector<std::string> failures{};

    while (std::getline(input, id_string))
    {
        std::cout.flush();
        
        auto const id_digits = create_asset_id(id_string);
        if (!id_digits)
        {
            failures.push_back(id_string);
            continue;
        }

        auto const checked_id = create_checked_asset_id(*id_digits);
        if (!checked_id)
        {
            failures.push_back(id_string);
            continue;
        }

        auto const output_file = (output_dir / id_string).replace_extension("png");

        if (!write_as_png(*checked_id, output_file))
        {
            failures.push_back(id_string);
            continue;
        }
    }

    if (!failures.empty())
    {
        std::cout << "ERROR: failures occurred:\n";
        for (auto const& failure: failures)
        {
            std::cout << "\t" << failure << std::endl;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
