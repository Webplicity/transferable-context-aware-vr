set(TARGET_NAME tracking)

if(APPLE)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_source_files_properties(../shared/pathtools.cpp main.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    find_library(FOUNDATION_FRAMEWORK Foundation)
    mark_as_advanced(FOUNDATION_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})
  endif()
endif()

add_executable(${TARGET_NAME}
  ${SHARED_SRC_FILES}
  main.cpp utils.h utils.cpp)

find_library(SDL2TTF_LIBRARIES SDL_ttf)

#set(CMAKE_CXX_FLAGS_DEBUG "SDL2/SDL_ttf")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "SDL2/SDL_ttf")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "SDL2/SDL_ttf")
#set(CMAKE_CXX_FLAGS_RELEASE "SDL2/SDL_ttf")
#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Wconversion")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
#find_package(PahoMqttCpp REQUIRED)


#set (MQTT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/paho.mqtt.cpp)

target_link_libraries(${TARGET_NAME}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${OPENVR_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ${EXTRA_LIBS}
  ${SDL2TFF_LIBRARIES}
  /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0
  #  SDL #bovenste lijn gebruiken ipv dit
  cppzmq
  paho-mqtt3a
  paho-cpp-objs
#  mqtt paho-mqtt3a
#  /home/charles/Documents/UA/vr_tracking/thirdparty/sdl2_ttf-2.0.14/lib/x86/SDL2_ttf.lib
#  SDL_ttf
#  ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/sdl2_ttf-2.0.14/lib/x64
)

setTargetOutputDirectory(${TARGET_NAME})

