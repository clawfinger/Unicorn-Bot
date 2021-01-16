cmake_minimum_required(VERSION 3.0)
set(PROJECT DiscordBot)
project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    Dispatcher.cpp
    Bot.cpp
    )

set(HEADERS
    DataTransferObjects.h
    Dispatcher.h
    Meta.h
    Bot.h
    )

add_executable(${PROJECT} ${SOURCES} ${HEADERS})
add_compile_definitions(_WEBSOCKETPP_CPP11_THREAD_)
add_subdirectory(Deps/sleepy-discord)
add_subdirectory(Deps/sqliteWrapper)
target_link_libraries(${PROJECT} sleepy-discord)
target_link_libraries(${PROJECT} sqliteWrapper)
