## Intruduction

This repo produces a command line tool `asset_id` that will process a list of 4 digit asset ids and output a png file per asset code. Each of these output files is a representation of the associated asset id that can be rendered on a 6 character, seven segement display.

The `asset_id` tool can be invoked in 2 ways:

```bash
asset_id <SOURCE_DATA> <DESTINATION_DIR>
asset_id
```

The first invocation will generate the png files, the second will display help text.

There are string limitations on the 2 parameters used in the first invocation:
1. SOURCE_DATA must be a readable text file.
1. DESTINATION_DIR must be a writeable directory that ALREADY exists.

The `asset_id` will fail immediately with a log message if either of these conditions is not met; no png files shall be generated.

Further, the SOURCE_DATA file must consist of 1 or more 4 decimal digit ids; each id must be on a separate line and should be padded on the left by zeroes. So `12` is not accceptable, `0012` is.

Any id that does not meet this specification will logged at the end of the call to `asset_id`; no png file will be generated for failed ids.

Note that the `asset_id` tool will not clear the DESTINATION_DIR before running; if files are present they will either be overwritten or written alongside.

## Development Environment

- Windows 11
- VS Code
- WSL2 (Ubuntu 24.04.2 LTS, 'noble')
- g++ 13.3.0
- Cmake 3.28.3
- ninja 1.11.1
- Ubuntu clang-format 18.1.3

VS Code extension

## External Dependencies

Catch2 (added as git submodule)
- Legacy version used to benefit from single header structure.

PNG support
- sudo apt-get install zlib1g-dev
- sudo apt install -y libpng-dev

## Initialise repo

Held in Github: https://github.com/MattHerring/asset-id.git

From the top level directory of the repo:

```bash
git submodule update --init --recursive
```

## Building

From the top level directory of the repo:

```bash
cmake -B ./build
cmake --build build
```

The tool artifact is located inside \build\src\asset_id\

## Run the tests

From the top level directory of the repo:

```bash
ctest --test-dir ./build/tests
```

## Integration testing

There are number of test scenarios setup in `test_scenarios`. Each one contains input data and a destination directory that should be used in an invocation of the `asset_id` tool:

```bash
./build/src/asset_id ./test_scenarios/<SCENARIO>/data.txt ./test_scenarios/<SCENARIO>/destination/
```
The results can be checked manually. These should be automated tests but are placed here for completeness. The scenarios are:

1. baseline_example: successfully generates a single png file for the documented example of '1337'.
1. multiple_inputs_all_errors: should log all ids as failed and produce no png files.
1. multiple_inputs_all_ok: should report no failures and produce all expected png files.
1. multiple_inputs_multiple_errors: should failures and produce some expected png files.
1. non_writeable_dest_dir: should fail reporting inaccessible destination with no png files.

Mistyping paths to either input or output parameters will trigger an 'inaccessibility' error with
no generated png files.

## Limitations

- Linux specific commands are used to interact with the filesystem so other operating systems are not supported.


## Appendix

The existing state of the integration tests are a little poor, they should be implemented as automated tests (most likely written as `pytest` fixtures that invoke the `asset_id` tool via the `process` module and check the results, this is a stretch goal). 
For ease of running here are the commands to call them:

```bash
./build/src/asset_id ./test_scenarios/baseline_example/data.txt ./test_scenarios/baseline_example/destination/
./build/src/asset_id ./test_scenarios/empty_input_generates_nothing/data.txt ./test_scenarios/empty_input_generates_nothing/destination/
./build/src/asset_id ./test_scenarios/multiple_inputs_all_errors/data.txt ./test_scenarios/multiple_inputs_all_errors/destination/
./build/src/asset_id ./test_scenarios/multiple_inputs_all_ok/data.txt ./test_scenarios/multiple_inputs_all_ok/destination/
./build/src/asset_id ./test_scenarios/multiple_inputs_multiple_errors/data.txt ./test_scenarios/multiple_inputs_multiple_errors/destination/

# Non-writeable files are difficult to commit to git (!).
# So revoke write access first (the directory is empty so no recursion 
# is necessary).
chmod -wx test_scenarios/non_writable_dest_dir/destination/
./build/src/asset_id ./test_scenarios/non_writable_dest_dir/data.txt ./test_scenarios/non_writable_dest_dir/destination/
chmod +wx test_scenarios/non_writable_dest_dir/destination/
```


