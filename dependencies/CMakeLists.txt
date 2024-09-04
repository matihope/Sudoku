# Huge thanks to https://github.com/eliasdaler for the base script!

include(FetchContent)

set(SFML_VERSION 2.6.1)
set(NLOHMANN_JSON_VERSION 3.11.3)

FetchContent_Declare(
        SFML
        URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)

FetchContent_Declare(
        json
        URL "https://github.com/nlohmann/json/archive/v${NLOHMANN_JSON_VERSION}.zip"
)

# SFML
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

# nlohmann-json
FetchContent_MakeAvailable(json)
