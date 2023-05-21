include(GitUpdate)
if (NOT GitUpdate_SUCCESS)
    return()
endif()

include(StaticRuntime)
include(GTestUtils)
include(ExternalTarget)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Template_BUILD_TEST          "Build the unit test program." ON)
option(Template_AUTO_RUN_TEST       "Automatically run the test program." ON)
option(Template_USE_STATIC_RUNTIME  "Build with the MultiThreaded(Debug) runtime library." ON)

if (Template_USE_STATIC_RUNTIME)
    set_static_runtime()
else()
    set_dynamic_runtime()
endif()


configure_gtest(${Template_SOURCE_DIR}/Test/googletest 
                ${Template_SOURCE_DIR}/Test/googletest/googletest/include)


set(Configure_SUCCEEDED TRUE)