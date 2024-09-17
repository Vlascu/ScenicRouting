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
## How to Run the Application

Follow these steps to set up, build, and run the application:

### 1. **Clone the Repository**
   First, clone the repository to your local machine:
   ```bash
   git clone https://github.com/yourusername/your-repo.git
   cd your-repo
   ```

### 2. **Run the DataBuilder project**
   In the main.cpp you will need to choose the starting point coordinates, end point coordinates and a desired walk time.
   Choose the naming of the .txt file that will be generated.
   Run the application and a .txt file will be generated in '../Output', this is will be the data for the optimization algorithm.

### 3. **Run the RoutingGenerator project**
   In the main.cpp make sure you choose the correct .txt file.
   Make sure the desired time variable matches the one that the data was built upon.
   Run the application and a .kml file will be generated in '../Output'.
   Import this .kml file into Google Earth and you will se the optimized route.
   
   
