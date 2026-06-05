cmake_minimum_required(VERSION 3.16)
project(todolist)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon_1024x1024.icns")

add_executable(todolist MACOSX_BUNDLE
        main.cpp
        ${APP_ICON}
)

set_source_files_properties(
        ${APP_ICON}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
)

set_target_properties(todolist PROPERTIES
        MACOSX_BUNDLE_ICON_FILE "icon_1024x1024.icns"

        MACOSX_BUNDLE_BUNDLE_NAME "TodoList"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.charly.todolist"

        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)

target_link_libraries(todolist PRIVATE Qt6::Widgets sqlite3)