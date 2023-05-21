set(TargetName Expression)
include(GroupSet)


set_group(Target_SRC
    Header:*.h
    Header:*.grm
    Header:*.inl
    Source:*.cpp
    )

include_directories(
    ${Expression_INCLUDE}
    ${Utils_INCLUDE} 
    ${Math_INCLUDE}
    ${ParserBase_INCLUDE} 
)

add_library(
    ${TargetName} 
    ${Target_SRC} 
)

target_link_libraries(
    ${TargetName} 
    ${Utils_LIBRARY} 
    ${Math_LIBRARY}
    ${ParserBase_LIBRARY} 
)

set_target_properties(
    ${TargetName} 
    PROPERTIES FOLDER "${TargetGroup}"
)
