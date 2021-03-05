cmake_minimum_required(VERSION 3.1)

set(ENGINE_PROJECT_NAME "SFMLEngine")
set(GAME_PROJECT_NAME "GameDevAndBeyond")
set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML/lib/cmake/SFML")
ELSE()
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML_GCC/lib/cmake/SFML")
ENDIF()

set(SFML_STATIC_LIBRARIES TRUE)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(nlohmann_json_subDir "Code/ThirdParty/json-3.9.1/")
add_subdirectory(${nlohmann_json_subDir})

set(tinyxml2_subDir "Code/ThirdParty/tinyxml2/")
add_subdirectory(${tinyxml2_subDir})

project(${ENGINE_PROJECT_NAME})
project(${GAME_PROJECT_NAME})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Code)

set(ENGINE_SOURCE_PATH Code/${ENGINE_PROJECT_NAME})
set(ENGINE_SOURCE_FILES 
${ENGINE_SOURCE_PATH}/Core/InputManager.h
${ENGINE_SOURCE_PATH}/Core/InputManager.cpp
${ENGINE_SOURCE_PATH}/Core/Engine.h
${ENGINE_SOURCE_PATH}/Core/Engine.cpp
${ENGINE_SOURCE_PATH}/Core/Interfaces/EngineTickListener.h
${ENGINE_SOURCE_PATH}/EntitySystem/EntityId.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntityId.h
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystem.cpp
${ENGINE_SOURCE_PATH}/EntitySystem/EntitySystem.h
${ENGINE_SOURCE_PATH}/ComponentSystem/ComponentPool.cpp
${ENGINE_SOURCE_PATH}/ComponentSystem/ComponentPool.h
${ENGINE_SOURCE_PATH}/ComponentSystem/ComponentSystem.cpp
${ENGINE_SOURCE_PATH}/ComponentSystem/ComponentSystem.h
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/SpriteRenderProperties.h
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/SpriteRenderSystem.cpp
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/SpriteRenderSystem.h
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/TextRenderProperties.h
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/TextRenderSystem.cpp
${ENGINE_SOURCE_PATH}/GameFramework/Rendering/TextRenderSystem.h
${ENGINE_SOURCE_PATH}/GameFramework/TransformProperties.h
${ENGINE_SOURCE_PATH}/GameFramework/TransformProperties.cpp
${ENGINE_SOURCE_PATH}/GameFramework/AudioProperties.h
${ENGINE_SOURCE_PATH}/GameFramework/AudioProperties.cpp
${ENGINE_SOURCE_PATH}/Serialization/IArchive.h
${ENGINE_SOURCE_PATH}/Serialization/JsonArchive.h
${ENGINE_SOURCE_PATH}/Serialization/JsonArchive.cpp
${ENGINE_SOURCE_PATH}/Serialization/BinArchive.h
${ENGINE_SOURCE_PATH}/Serialization/BinArchive.cpp
${ENGINE_SOURCE_PATH}/Serialization/VectorInterface.h
${ENGINE_SOURCE_PATH}/Serialization/ObjectInterface.h
${ENGINE_SOURCE_PATH}/FightingEngine/Fighter.h
${ENGINE_SOURCE_PATH}/FightingEngine/Fighter.cpp
${ENGINE_SOURCE_PATH}/FightingEngine/Lyn.h
${ENGINE_SOURCE_PATH}/FightingEngine/SailorMoon.h
${ENGINE_SOURCE_PATH}/FightingEngine/AnimatorSystem.h
${ENGINE_SOURCE_PATH}/FightingEngine/AnimatorSystem.cpp
${ENGINE_SOURCE_PATH}/FightingEngine/InputSystem.h
${ENGINE_SOURCE_PATH}/FightingEngine/InputSystem.cpp
${ENGINE_SOURCE_PATH}/FightingEngine/LogicSystem.h
${ENGINE_SOURCE_PATH}/FightingEngine/LogicSystem.cpp
${ENGINE_SOURCE_PATH}/FightingEngine/TransformSystem.h
${ENGINE_SOURCE_PATH}/FightingEngine/TransformSystem.cpp
${ENGINE_SOURCE_PATH}/FightingEngine/CharacterEditor.h
${ENGINE_SOURCE_PATH}/FightingEngine/CharacterEditor.cpp
)

set(GAME_SOURCE_PATH Code/${GAME_PROJECT_NAME})
set(GAME_SOURCE_FILES 
${GAME_SOURCE_PATH}/main.cpp
${GAME_SOURCE_PATH}/Game.cpp
${GAME_SOURCE_PATH}/Game.h
)

add_library(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE_FILES})
add_executable(${GAME_PROJECT_NAME} ${GAME_SOURCE_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_PROJECT_NAME})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${GAME_SOURCE_PATH} FILES ${GAME_SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${ENGINE_SOURCE_PATH} FILES ${ENGINE_SOURCE_FILES})

target_link_libraries(${ENGINE_PROJECT_NAME} sfml-graphics sfml-audio nlohmann_json::nlohmann_json tinyxml2)
target_link_libraries(${GAME_PROJECT_NAME} ${ENGINE_PROJECT_NAME} )