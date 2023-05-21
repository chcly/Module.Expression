cmake_minimum_required(VERSION 3.15)
project(Expression)

# Use C++-17 by default.
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set the module path to include scripts
# CMake - Contains project local modules.
#
# CMake/Modules - Contains project independent modules.
#   GitUpdate.cmake invokes python ${Expression_SOURCE_DIR}/gitupdate.py
#   to pull all submodules and update to head.
#   If this fails, it will attempt to invoke git directly to pull 
#   submodules. 
list(APPEND CMAKE_MODULE_PATH 
  ${Expression_SOURCE_DIR}/CMake 
  ${Expression_SOURCE_DIR}/CMake/Modules
)
include(Configure)
if (NOT Configure_SUCCEEDED)
   return()
endif()



if (Expression_ExternalTarget)
    set(TargetFolders ${Expression_TargetFolders})
    set(TargetName    ${Expression_TargetName})
    set(TargetGroup   ${Expression_TargetGroup})
else()
    add_subdirectory(CMake)
    add_subdirectory(Internal/Utils)
    include(${Utils_SOURCE_DIR}/CMake/Locals.cmake)
    add_subdirectory(Internal/Math)
    add_subdirectory(Internal/ParserBase)

    set(TargetFolders TRUE)
    set(TargetName Expression)
    set(TargetGroup )

    set(Expression_INCLUDE  ${Expression_SOURCE_DIR})
    set(Expression_LIBRARY  ${TargetName})
endif()

add_subdirectory(Expression)

if (Expression_BUILD_TEST)
    if (NOT Expression_ExternalTarget)
        add_gtest_source(Extern/ThirdParty)
    endif()

    set(TargetGroup Units)
    add_subdirectory(Test)
endif()
