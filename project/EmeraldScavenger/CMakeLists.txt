# Add hpp and cpp files to Snake variable
file(GLOB EmeraldScavenger
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(EmeraldScavenger ${EmeraldScavenger} Enemy.hpp Door.cpp Door.hpp CollectibleItem.cpp CollectibleItem.hpp)
target_link_libraries(EmeraldScavenger ${all_libs})

# copy files to dest
file(COPY
        assets/background.png
        assets/start.png
        assets/fireSprites.json assets/fireSprites.png
        assets/enemiesSprites.json assets/enemiesSprites.png
        assets/obstacles.json assets/obstacles.png
        assets/scavengerStages.json assets/scavengerStages.png
        assets/platformer-art-deluxe.json assets/platformer-art-deluxe.png
        assets/gameSprites.json assets/gameSprites.png
        assets/ui.json assets/ui.png
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY
        assets/background.png
        assets/start.png
        assets/fireSprites.json assets/fireSprites.png
        assets/enemiesSprites.json assets/enemiesSprites.png
        assets/obstacles.json assets/obstacles.png
        assets/scavengerStages.json assets/scavengerStages.png
        assets/platformer-art-deluxe.json assets/platformer-art-deluxe.png
        assets/gameSprites.json assets/gameSprites.png
        assets/ui.json assets/ui.png
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)


    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(EmeraldScavenger PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF (WIN32)
