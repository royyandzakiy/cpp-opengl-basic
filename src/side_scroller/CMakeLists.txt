# =============================================================================
# ${MY_PROJECT_NAME} — Standalone Hello-World Binary
# =============================================================================
# Intentionally independent of MyLib so BUILD_APP and BUILD_LIB can be toggled
# separately in multi-config workflows (e.g. CI matrix, feature toggles).
# =============================================================================

# ---- Executable -------------------------------------------------------------
set(MY_PROJECT_NAME side_scroller)
add_executable(${MY_PROJECT_NAME})

# ---- Sources ----------------------------------------------------------------
target_sources(${MY_PROJECT_NAME}
    PRIVATE
        main.cpp
)

# ---- Include Paths ----------------------------------------------------------
target_include_directories(${MY_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---- Properties -------------------------------------------------------------
set_target_properties(${MY_PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF           # Matches root setting (good practice)
    # OUTPUT_NAME ${MY_PROJECT_NAME}          # Optional: lower-case output name
    # VERSION ${PROJECT_VERSION} # Optional: version info
)

# ---- Compile Features -------------------------------------------------------
# Even though root sets it globally, this makes it explicit
target_compile_features(${MY_PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

# ---- Compile Options --------------------------------------------------------
# target_compile_options(${MY_PROJECT_NAME}
#     PRIVATE
#         $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
#         $<$<CXX_COMPILER_ID:GCC>:-Wno-sign-compare>
# )

# ---- Dependencies -----------------------------------------------------------
target_link_libraries(${MY_PROJECT_NAME}
    PRIVATE
        fmt::fmt
        GLEW::GLEW
        glfw
        OpenGL::GL
)

# ---- Cross-Cutting Configuration --------------------------------------------
# Applies PCH, unity build, profiler links (Tracy/Perfetto), clang-tidy,
# Windows ASan DLL deployment — all via configure_target().
# See cmake/configure_target.cmake for implementation details.
configure_target(${MY_PROJECT_NAME})

message(STATUS "${MY_PROJECT_NAME} configured (standalone executable)")
