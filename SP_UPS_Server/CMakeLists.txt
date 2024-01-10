cmake_minimum_required(VERSION 3.26)
project(SP_UPS_Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


add_executable(SP_UPS_Server main.cpp Server.cpp Server.h Player.cpp Player.h ClientManager.cpp ClientManager.h Request.cpp Request.h Utils.cpp Utils.h Response.cpp Response.h GameManager.cpp GameManager.h Game.cpp Game.h GameLogic.cpp GameLogic.h
        Game.cpp)

configure_file(configfile.txt ${CMAKE_BINARY_DIR}/configfile.txt COPYONLY)