cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6.4)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake ${CMAKE_HOME_DIRECTORY}/GG/cmake)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

########################################
# Configuration                        #
########################################
project(FreeOrion)

set(FreeOrion_VERSION 0.4.3+)
set(FREEORION_RELEASE false)

set(MINIMUM_BOOST_VERSION 1.47.0)

option(BUILD_TESTS "Controls generation of unit tests." OFF)

if (BUILD_TESTS)
    enable_testing()
endif ()

find_package (Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/freeorion")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (WIN32)
    add_definitions(
        -D_CRT_SECURE_NO_DEPRECATE
        -D_SCL_SECURE_NO_DEPRECATE
        -D_CRT_NONSTDC_NO_DEPRECATE
        -DBOOST_ALL_DYN_LINK
        -DLOG4CPP_STLPORT_AND_BOOST_BUILD
        -DFREEORION_WIN32
        -D_WIN32_WINNT=0x0501
    )
    if (MSVC)
        set (CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../")
        set (BOOST_ROOT "${CMAKE_SOURCE_DIR}/../Boost/")
        set (BOOST_INCLUDEDIR "${CMAKE_SOURCE_DIR}/../Boost/include/boost_1_51/")
        set (OIS_INC_SEARCH_PATH "${CMAKE_SOURCE_DIR}/OIS/")
        set (CMAKE_STATIC_LIBRARY_PREFIX "lib")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4099 /wd4101 /wd4146 /wd4244 /wd4251 /wd4258 /wd4267 /wd4275 /wd4351 /wd4800 /wd4996")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
    endif ()
elseif (APPLE)
    add_definitions(-DFREEORION_MACOSX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(
        -DFREEORION_LINUX
        -DENABLE_BINRELOC
    )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wno-parentheses")
elseif (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    add_definitions(
        -DFREEORION_FREEBSD
        -DENABLE_BINRELOC
    )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wno-parentheses")
else ()
    message(FATAL_ERROR "Unknown platform type! Your setup is not a supported platform for FreeOrion.")
endif ()

configure_file(
    ${CMAKE_SOURCE_DIR}/ogre_plugins.cfg.in
    ${CMAKE_BINARY_DIR}/ogre_plugins.cfg
)

find_package(Subversion)
set(FreeOrion_WC_REVISION ???)
set(FreeOrion_BUILDSYS "CMake")
if (Subversion_FOUND AND (
  IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.svn" OR
  IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../.svn" OR
  IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../../.svn")
)
    Subversion_WC_INFO("${CMAKE_SOURCE_DIR}" FreeOrion)
endif ()
configure_file(
    ${CMAKE_SOURCE_DIR}/util/Version.cpp.in
    ${CMAKE_BINARY_DIR}/util/Version.cpp
)

if("${FreeOrion_WC_REVISION}" STREQUAL "???")
    set(FreeOrion_WC_REVISION "unknown")
endif ()

if (WIN32)
    # Add icon resource file to freeorion.exe
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/win32_resources.rc)
        file(WRITE ${CMAKE_BINARY_DIR}/win32_resources.rc "IDI_ICON ICON \"${CMAKE_SOURCE_DIR}/FreeOrion.ico\"")
    endif ()
endif ()

########################################
# Build common code library            #
########################################
set(BUILD_SDL_DRIVER OFF CACHE INTERNAL "Disable building of SDL driver." FORCE)
set(BUILD_DEVEL_PACKAGE OFF CACHE INTERNAL "Disables installation of GiGi development files." FORCE)
set(OLD_LIB_SUFFIX ${LIB_SUFFIX})
set(LIB_SUFFIX "${LIB_SUFFIX}/freeorion")
add_subdirectory(GG)
set(LIB_SUFFIX ${OLD_LIB_SUFFIX})

########################################
# Dependencies                         #
########################################
find_package(Boost ${MINIMUM_BOOST_VERSION} COMPONENTS chrono date_time filesystem regex serialization signals system thread REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
    GG
    log4cpp
    OpenSteer/include
)

add_definitions(-DADOBE_STD_SERIALIZATION)

if (FREEORION_RELEASE)
    add_definitions(-DFREEORION_RELEASE)
endif ()

set (freeorioncommon_HEADER
    combat/AsteroidBeltObstacle.h
    combat/CombatEventListener.h
    combat/CombatFighter.h
    combat/CombatLogManager.h
    combat/CombatObject.h
    combat/CombatOrder.h
    combat/CombatShip.h
    combat/CombatSystem.h
    combat/Missile.h
    combat/PathingEngineFwd.h
    combat/PathingEngine.h
    combat/ProximityDatabase.h
    Empire/Diplomacy.h
    Empire/Empire.h
    Empire/EmpireManager.h
    Empire/ResourcePool.h
    network/Message.h
    network/MessageQueue.h
    network/Networking.h
    python/PythonSetWrapper.h
    python/PythonWrappers.h
    universe/Building.h
    universe/Condition.h
    universe/EffectAccounting.h
    universe/Effect.h
    universe/Enums.h
    universe/Field.h
    universe/Fleet.h
    universe/Meter.h
    universe/ObjectMap.h
    universe/Planet.h
    universe/PopCenter.h
    universe/Predicates.h
    universe/PythonUniverseGenerator.h
    universe/ResourceCenter.h
    universe/ShipDesign.h
    universe/Ship.h
    universe/Special.h
    universe/Species.h
    universe/System.h
    universe/Tech.h
    universe/Universe.h
    universe/UniverseObject.h
    universe/TemporaryPtr.h
    universe/ValueRef.h
    universe/ValueRefFwd.h
    util/AppInterface.h
    util/blocking_combiner.h
    util/DataTable.h
    util/Directories.h
    util/i18n.h
    util/Logger.h
    util/Math.h
    util/ModeratorAction.h
    util/MultiplayerCommon.h
    util/OptionsDB.h
    util/OptionValidators.h
    util/Order.h
    util/OrderSet.h
    util/Process.h
    util/Random.h
    util/ScopedTimer.h
    util/Serialize.h
    util/Serialize.ipp
    util/SitRepEntry.h
    util/StringTable.h
    util/VarText.h
    util/Version.h
    util/XMLDoc.h
)

set (freeorioncommon_SOURCE
    combat/AsteroidBeltObstacle.cpp
    combat/CombatFighter.cpp
    combat/CombatLogManager.cpp
    combat/CombatObject.cpp
    combat/CombatOrder.cpp
    combat/CombatShip.cpp
    combat/Missile.cpp
    combat/PathingEngine.cpp
    Empire/Diplomacy.cpp
    Empire/Empire.cpp
    Empire/EmpireManager.cpp
    Empire/ResourcePool.cpp
    network/Message.cpp
    network/MessageQueue.cpp
    network/Networking.cpp
    OpenSteer/src/Obstacle.cpp
    OpenSteer/src/SimpleVehicle.cpp
    OpenSteer/src/Vec3.cpp
    OpenSteer/src/Vec3Utilities.cpp
    universe/Building.cpp
    universe/Condition.cpp
    universe/EffectAccounting.cpp
    universe/Effect.cpp
    universe/Enums.cpp
    universe/Field.cpp
    universe/Fleet.cpp
    universe/Meter.cpp
    universe/ObjectMap.cpp
    universe/Planet.cpp
    universe/PopCenter.cpp
    universe/Predicates.cpp
    universe/ResourceCenter.cpp
    universe/Ship.cpp
    universe/ShipDesign.cpp
    universe/Special.cpp
    universe/Species.cpp
    universe/System.cpp
    universe/Tech.cpp
    universe/Universe.cpp
    universe/UniverseObject.cpp
    universe/ValueRef.cpp
    util/AppInterface.cpp
    util/DataTable.cpp
    util/Directories.cpp
    util/i18n.cpp
    util/Logger.cpp
    util/Math.cpp
    util/ModeratorAction.cpp
    util/MultiplayerCommon.cpp
    util/OptionsDB.cpp
    util/Order.cpp
    util/OrderSet.cpp
    util/Process.cpp
    util/Random.cpp
    util/ScopedTimer.cpp
    util/SerializeEmpire.cpp
    util/SerializeModeratorAction.cpp
    util/SerializeMultiplayerCommon.cpp
    util/SerializeOrderSet.cpp
    util/SerializePathingEngine.cpp
    util/SerializeUniverse.cpp
    util/SitRepEntry.cpp
    util/StringTable.cpp
    util/VarText.cpp
    util/XMLDoc.cpp
    util/Version.cpp.in
    ${CMAKE_BINARY_DIR}/util/Version.cpp
)

if (NOT WIN32)
    list(APPEND freeorioncommon_SOURCE
        util/binreloc.c
    )
endif ()

add_library(freeorioncommon
    ${freeorioncommon_HEADER}
    ${freeorioncommon_SOURCE}
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set_property(
        SOURCE
        util/SerializePathingEngine.cpp
        PROPERTY
        COMPILE_FLAGS -ftemplate-depth=512
    )
endif ()

target_link_libraries(freeorioncommon
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENGL_LIBRARIES}
    log4cpp
)

set_target_properties(freeorioncommon
    PROPERTIES
    DEFINE_SYMBOL "FREEORION_BUILD_COMMON"
)

if (CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(freeorioncommon
        PROPERTIES
        COMPILE_FLAGS "-fvisibility=hidden"
)
endif ()

########################################
# Recurse Into Sources                 #
########################################
add_subdirectory(log4cpp)
add_subdirectory(PagedGeometry)
add_subdirectory(parse)
add_subdirectory(server)
add_subdirectory(client/AI)
add_subdirectory(client/human)

########################################
# Packaging                            #
########################################
set(BUILD_PACKAGING
    false
    CACHE BOOL
    "Whether to build installer/binary package. (This is the executable installer on Win32, and an RPM/DEB/tarball on Linux."
)

if (WIN32)
    set(BUILD_SDK
        false
        CACHE BOOL
        "Whether to build the Win32 SDK installer."
    )
endif ()

if (BUILD_SDK)
    install(
        DIRECTORY ../FreeOrion
        DESTINATION .
        COMPONENT COMPONENT_FREEORION_SOURCE
        PATTERN "*~" EXCLUDE
    )

    install(
        DIRECTORY ../Boost
        DESTINATION .
        COMPONENT COMPONENT_FREEORION_BUILD_FILES_BOOST
        PATTERN "*~" EXCLUDE
    )

    install(
        DIRECTORY ../include
        DESTINATION .
        COMPONENT COMPONENT_FREEORION_BUILD_FILES_INCLUDE
        PATTERN "*~" EXCLUDE
    )

    install(
        DIRECTORY ../lib
        DESTINATION .
        COMPONENT COMPONENT_FREEORION_BUILD_FILES_LIB
        PATTERN "*~" EXCLUDE
    )

    install(
        FILES
        ../build/GG/CMakeCache.txt
        DESTINATION build/GG
        COMPONENT COMPONENT_FREEORION_BUILD_FILES_GIGI
    )

    install(
        FILES
        ../build/CMakeCache.txt
        DESTINATION build
        COMPONENT COMPONENT_FREEORION_BUILD_FILES_FREEORION
    )

    install(
        FILES
        ../README.txt
        ../vcredist_x86.exe
        ../cmake-2.8.0-win32-x86.exe
        DESTINATION .
        COMPONENT COMPONENT_FREEORION_MISC_SDK_FILES
    )
else ()
    install(
        DIRECTORY default/
        DESTINATION share/freeorion/default
        COMPONENT COMPONENT_FREEORION_DATA
        PATTERN ".svn" EXCLUDE
        PATTERN "*~" EXCLUDE
        PATTERN "*.pyc" EXCLUDE
    )

    install(
        TARGETS freeorioncommon
        LIBRARY DESTINATION lib${LIB_SUFFIX}/freeorion
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_BINARY_DIR}/ogre_plugins.cfg
        DESTINATION share/freeorion
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/OISInput.cfg
        DESTINATION share/freeorion/default
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/freeorion.desktop
        DESTINATION share/applications
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_16x16.png
        DESTINATION share/icons/hicolor/16x16/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_24x24.png
        DESTINATION share/icons/hicolor/24x24/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_32x32.png
        DESTINATION share/icons/hicolor/32x32/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_64x64.png
        DESTINATION share/icons/hicolor/64x64/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_128x128.png
        DESTINATION share/icons/hicolor/128x128/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    install(
        FILES
        ${CMAKE_SOURCE_DIR}/default/data/art/icons/FO_Icon_256x256.png
        DESTINATION share/icons/hicolor/256x256/apps/
        RENAME freeorion.png
        COMPONENT COMPONENT_FREEORION
    )

    if (WIN32)
        install(
            FILES
            OIS.dll
            OgreMain.dll
            OpenAL32.dll
            Plugin_OctreeSceneManager.dll
            Plugin_ParticleFX.dll
            RenderSystem_GL.dll
            boost_date_time-vc90-mt-1_44.dll
            boost_filesystem-vc90-mt-1_44.dll
            boost_iostreams-vc90-mt-1_44.dll
            boost_python-vc90-mt-1_44.dll
            boost_regex-vc90-mt-1_44.dll
            boost_serialization-vc90-mt-1_44.dll
            boost_signals-vc90-mt-1_44.dll
            boost_system-vc90-mt-1_44.dll
            boost_thread-vc90-mt-1_44.dll
            glew32.dll
            jpeg.dll
            libexpat.dll
            libogg.dll
            libpng13.dll
            libvorbis.dll
            libvorbisfile.dll
            python26.dll
            wrap_oal.dll
            z.dll
            zlib1.dll
            DESTINATION bin
            COMPONENT COMPONENT_FREEORION_WIN32_RUNTIME_DEPS
        )
    endif ()
endif ()

if (BUILD_SDK)
    set(CPACK_PACKAGE_VERSION "1.0")
else ()
    set(CPACK_PACKAGE_VERSION ${FreeOrion_VERSION})
endif ()

set(CPACK_PACKAGE_VENDOR "FreeOrion Community")
set(CPACK_PACKAGE_CONTACT http://freeorion.org/forum)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FreeOrion is a free, open source, turn-based space empire and galactic conquest (4X) computer game being designed and built by the FreeOrion project. FreeOrion is inspired by the tradition of the Master of Orion games, but is not a clone or remake of that series or any other game.")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_HOME_DIRECTORY}/default/COPYING)
if (BUILD_SDK)
    set(CPACK_RESOURCE_FILE_README ${CMAKE_HOME_DIRECTORY}/../README.txt)
endif ()
if (BUILD_PACKAGING)
    set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
    if (WIN32)
        set(PACKAGE_FILE_SYSTEM_NAME win32)
    else ()
        set(PACKAGE_FILE_SYSTEM_NAME ${CPACK_SYSTEM_NAME})
    endif ()
    if (BUILD_SDK)
        set(CPACK_PACKAGE_NAME "FreeOrion SDK")
        set(CPACK_PACKAGE_FILE_NAME "FreeOrion_Win32_SDK")
        string(CONFIGURE "FreeOrion_SDK_${CPACK_PACKAGE_VERSION}" dir)
        set(CPACK_PACKAGE_INSTALL_DIRECTORY ${dir})
    else ()
        set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_FILE_SYSTEM_NAME}")
        string(TOLOWER ${CPACK_PACKAGE_FILE_NAME} CPACK_PACKAGE_FILE_NAME)
    endif ()
    set(CPACK_MONOLITHIC_INSTALL ON)
endif ()

# NSIS-specific settings
set(CPACK_NSIS_COMPRESSOR bzip2)
set(CPACK_NSIS_URL_INFO_ABOUT http://freeorion.org)
string(REPLACE "/" "\\\\" cmake_home_directory_with_native_windows_path_separators ${CMAKE_HOME_DIRECTORY})

if (BUILD_SDK)
    string(REGEX REPLACE "/FreeOrion$" "" sdk_root ${CMAKE_HOME_DIRECTORY})
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        !include \\\"WordFunc.nsh\\\"

        StrCpy $7 $INSTDIR
        \\\${WordReplace} $7 \\\"\\\\\\\" \\\"/\\\" \\\"+\\\" $8

	Rename $INSTDIR\\\\build\\\\GG\\\\CMakeCache.txt $INSTDIR\\\\build\\\\GG\\\\CMakeCache.txt.in
        FileOpen $0 $INSTDIR\\\\build\\\\GG\\\\CMakeCache.txt.in r
        FileOpen $5 $INSTDIR\\\\build\\\\GG\\\\CMakeCache.txt w
        ClearErrors
        loop1:
            FileRead $0 $1
            IfErrors done1
            \\\${WordReplace} $1 \\\"${sdk_root}\\\" $8 \\\"+\\\" $2
            FileWrite $5 $2
            Goto loop1
        done1:
        FileClose $0
        FileClose $5
        Delete $INSTDIR\\\\build\\\\GG\\\\CMakeCache.txt.in

	Rename $INSTDIR\\\\build\\\\CMakeCache.txt $INSTDIR\\\\build\\\\CMakeCache.txt.in
        FileOpen $0 $INSTDIR\\\\build\\\\CMakeCache.txt.in r
        FileOpen $5 $INSTDIR\\\\build\\\\CMakeCache.txt w
        ClearErrors
        loop2:
            FileRead $0 $1
            IfErrors done2
            \\\${WordReplace} $1 \\\"${sdk_root}\\\" $8 \\\"+\\\" $2
            FileWrite $5 $2
            Goto loop2
        done2:
        FileClose $0
        FileClose $5
        Delete $INSTDIR\\\\build\\\\CMakeCache.txt.in

        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\FreeOrion SDK.lnk\\\" \\\"$INSTDIR\\\"
        WriteIniStr \\\"$INSTDIR\\\\freeorion.url\\\" \\\"InternetShortcut\\\" \\\"URL\\\" \\\"http://freeorion.org\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\README.lnk\\\" \\\"$INSTDIR\\\\README.txt\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Website.lnk\\\" \\\"$INSTDIR\\\\freeorion.url\\\"
        File \\\"${cmake_home_directory_with_native_windows_path_separators}\\\\..\\\\cmake-2.8.0-win32-x86.exe\\\"
        ExecWait \\\"$INSTDIR\\\\cmake-2.8.0-win32-x86.exe\\\""
    )
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\FreeOrion SDK.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\README.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\Website.lnk\\\"
        RMDir /r /REBOOTOK $INSTDIR"
    )
else ()
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        File \\\"${cmake_home_directory_with_native_windows_path_separators}\\\\..\\\\vcredist_x86.exe\\\"
        ExecWait \\\"$INSTDIR\\\\vcredist_x86.exe /q\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\FreeOrion.lnk\\\" \\\"$INSTDIR\\\\freeorion.exe\\\" \\\"--fullscreen 1\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\FreeOrion windowed.lnk\\\" \\\"$INSTDIR\\\\freeorion.exe\\\"
        CreateShortCut \\\"$DESKTOP\\\\FreeOrion.lnk\\\" \\\"$INSTDIR\\\\freeorion.exe\\\" \\\"--fullscreen 1\\\"
        WriteIniStr \\\"$INSTDIR\\\\freeorion.url\\\" \\\"InternetShortcut\\\" \\\"URL\\\" \\\"http://freeorion.org\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Website.lnk\\\" \\\"$INSTDIR\\\\freeorion.url\\\""
    )
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\Website.lnk\\\"
        Delete \\\"$DESKTOP\\\\FreeOrion.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\FreeOrion.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\FreeOrion windowed.lnk\\\""
    )
endif ()

# RPM-specific settings
set(CPACK_RPM_PACKAGE_LICENSE GPL)
set(CPACK_RPM_PACKAGE_REQUIRES)
# TODO

# Deb-specific settings
set(CPACK_DEBIAN_PACKAGE_SECTION games)
# TODO

if (UNIX)
    set(CPACK_GENERATOR "TGZ;TBZ2")
    find_program(RPMBUILD rpmbuild)
    if (RPMBUILD)
        list(APPEND CPACK_GENERATOR RPM)
    endif ()
    set(RPMBUILD ${RPMBUILD} CACHE INTERNAL "")
    find_program(DPKG dpkg)
    if (DPKG)
        list(APPEND CPACK_GENERATOR DEB)
    endif ()
    set(DPKG ${DPKG} CACHE INTERNAL "")
elseif (WIN32)
    set(CPACK_GENERATOR "NSIS")
endif ()


########################################
# Source Packaging                     #
########################################

if (UNIX)
    set(CPACK_SOURCE_GENERATOR "TGZ")
elseif (WIN32)
    set(CPACK_SOURCE_GENERATOR "ZIP")
endif ()

set(CPACK_SOURCE_IGNORE_FILES
    "~$"
    "\\\\.asm$"
    "\\\\.bz2$"
    "/CMakeCache\\\\.txt$"
    "/CMakeFiles/"
    "/cmake_install\\\\.cmake$"
    "/CPackConfig.cmake$"
    "/_CPack_Packages/"
    "/CPackSourceConfig.cmake$"
    "/CTestTestfile\\\\.cmake$"
    "\\\\.dll$"
    "\\\\.exe$"
    "\\\\.exp$"
    "/freeorion$"
    "/freeorionca$"
    "/freeoriond$"
    "GG/GG/Config.h$"
    "\\\\.git/"
    "\\\\.gz$"
    "\\\\.lib$"
    "/Makefile$"
    "/ogre_plugins.cfg$"
    "\\\\.pc$"
    "\\\\.pdb$"
    "\\\\.pyc$"
    "/Release/"
    "\\\\.so$"
    "\\\\.swp$"
)

set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-v${FreeOrion_VERSION}-${FreeOrion_WC_REVISION}-source"
)

include(CPack)

if (BUILD_SDK)
    cpack_add_component(
        COMPONENT_FREEORION_SOURCE
        DISPLAY_NAME FreeOrion Source Code
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )
    cpack_add_component(
        COMPONENT_FREEORION_BUILD_FILES_BOOST
        DISPLAY_NAME FreeOrion Build Files -- Boost Directory
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION_BUILD_FILES_INCLUDE
        DISPLAY_NAME FreeOrion Build Files -- Include Directory
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION_BUILD_FILES_LIB
        DISPLAY_NAME FreeOrion Build Files -- Lib Directory
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION_BUILD_FILES_GIGI
        DISPLAY_NAME FreeOrion Build Files -- Gigi CMake Cache
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION_BUILD_FILES_FREEORION
        DISPLAY_NAME FreeOrion Build Files -- FreeOrion CMake Cache
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION_MISC_SDK_FILES
        DISPLAY_NAME FreeOrion Misc SDK Files
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )
else ()
    if (WIN32)
        cpack_add_component(
            COMPONENT_FREEORION_WIN32_RUNTIME_DEPS
            DISPLAY_NAME FreeOrion Win32 Runtime Dependencies
            DESCRIPTION ""
            HIDDEN
            REQUIRED
        )
    endif ()

    cpack_add_component(
        COMPONENT_FREEORION_DATA
        DISPLAY_NAME FreeOrion Data
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORION
        DISPLAY_NAME FreeOrion Client
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORIOND
        DISPLAY_NAME FreeOrion Server
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )

    cpack_add_component(
        COMPONENT_FREEORIONCA
        DISPLAY_NAME FreeOrion AI
        DESCRIPTION ""
        HIDDEN
        REQUIRED
    )
endif ()
