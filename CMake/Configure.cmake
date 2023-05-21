include(GitUpdate)
if (NOT GitUpdate_SUCCESS)
    return()
endif()

include(StaticRuntime)
include(GTestUtils)
include(ExternalTarget)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Expression_BUILD_TEST          "Build the unit test program." ON)
option(Expression_AUTO_RUN_TEST       "Automatically run the test program." ON)
option(Expression_USE_STATIC_RUNTIME  "Build with the MultiThreaded(Debug) runtime library." ON)

if (Expression_USE_STATIC_RUNTIME)
    set_static_runtime()
else()
    set_dynamic_runtime()
endif()


configure_gtest(${Expression_SOURCE_DIR}/Test/googletest 
                ${Expression_SOURCE_DIR}/Test/googletest/googletest/include)


DefineExternalTargetEx(
    Utils Extern
    ${Expression_SOURCE_DIR}/Internal/Utils 
    ${Expression_SOURCE_DIR}/Internal/Utils
    ${Expression_BUILD_TEST}
    ${Expression_AUTO_RUN_TEST}
)


DefineExternalTargetEx(
    Math Extern
    ${Expression_SOURCE_DIR}/Internal/Math
    ${Expression_SOURCE_DIR}/Internal/Math
    ${Expression_BUILD_TEST}
    ${Expression_AUTO_RUN_TEST}
)

DefineExternalTargetEx(
    ParserBase Extern
    ${Expression_SOURCE_DIR}/Internal/ParserBase 
    ${Expression_SOURCE_DIR}/Internal/ParserBase
    ${Expression_BUILD_TEST}
    ${Expression_AUTO_RUN_TEST}
)
                
set(Configure_SUCCEEDED TRUE)