project(k15_lua_standalone)

include_directories(../../dependencies/luajit/src)

add_executable(lua_standalone "./src/ParserMain.cpp")

target_link_libraries(lua_standalone luajit)