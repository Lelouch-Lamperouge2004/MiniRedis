cmake_minimum_required(VERSION 3.10)

project(MiniRedis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES OFF)
set(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "")

enable_testing()

set(MINI_REDIS_SOURCES
    src/Node.cpp
    src/DoublyLinkedList.cpp
    src/LRUCache.cpp
    src/PersistenceManager.cpp
)

add_executable(MiniRedis
    src/main.cpp
    src/CommandParser.cpp
    src/TCPServer.cpp
    ${MINI_REDIS_SOURCES}
)

target_link_libraries(MiniRedis
    ws2_32
)

find_package(GTest REQUIRED)

add_executable(MiniRedisTests
    tests/cache_test.cpp
    ${MINI_REDIS_SOURCES}
)

target_link_libraries(MiniRedisTests
    GTest::gtest
    GTest::gtest_main
)

add_test(
    NAME MiniRedisTests
    COMMAND MiniRedisTests
)

add_executable(MiniRedisStressTest
    tests/stress_test.cpp
)

target_link_libraries(MiniRedisStressTest
    ws2_32
)