# ScenicRouting
C++ app where OSM points of interest are downloaded based on start-end coordinates and desired walking time and then a meta-heuristic algorithm will build the most efficient route between these POIs maximizing the total priority of the POIs and minimizing the walk time. 

## Requirements

This project requires the following libraries:

1. **libcurl**: Used for making HTTP requests.
   - Installation on Linux (Debian/Ubuntu):
     ```bash
     sudo apt-get install libcurl4-openssl-dev
     ```
   - Installation on macOS (Homebrew):
     ```bash
     brew install curl
     ```
   - On Windows, you may need to download and build from [libcurl website](https://curl.se/libcurl/).

2. **nlohmann/json**: Used for handling JSON data.
   - You can download it from the [nlohmann/json GitHub page](https://github.com/nlohmann/json) or install via package managers:
     - Installation using vcpkg:
       ```bash
       vcpkg install nlohmann-json
       ```
     - Or you can include the single header file:
       ```cpp
       #include <nlohmann/json.hpp>
       ```

