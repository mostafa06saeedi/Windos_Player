cmake_minimum_required(VERSION 3.16)

project(play VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# پیدا کردن Qt6 با کامپوننت‌های مورد نیاز
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# پوشه‌ی هدرها و فایل‌های .ui برای AUTOUIC/AUTOMOC
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# لیست فایل‌های پروژه
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
)

# اضافه کردن executable با Qt6
qt_add_executable(play
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    include/musicplayer.h
    src/musicplayer.cpp
    include/user.h src/user.cpp
    include/simple_sync.h src/simple_sync.cpp
    include/equalizerwidget.h src/equalizerwidget.cpp
    include/login.h src/login.cpp
    include/usermanager.h src/usermanager.cpp
    include/encryption.h src/encryption.cpp
    include/clickableslider.h src/clickableslider.cpp
    ui/login.ui
)

# لینک دادن کتابخانه‌های Qt6 مورد نیاز
target_link_libraries(play
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia
)

# تنظیمات مخصوص macOS و Windows
if(DEFINED Qt6_VERSION AND Qt6_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.play)
endif()

set_target_properties(play PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# نصب هدف
include(GNUInstallDirs)
install(TARGETS play
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# نهایی کردن Qt6 executable
qt_finalize_executable(play)