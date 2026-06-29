# =============================================================================
# MyApp — Standalone Hello-World Binary
# =============================================================================
# Intentionally independent of MyLib so BUILD_APP and BUILD_LIB can be toggled
# separately in multi-config workflows (e.g. CI matrix, feature toggles).
# =============================================================================

# ---- Executable -------------------------------------------------------------
add_executable(MyApp)

# ---- Sources ----------------------------------------------------------------
target_sources(MyApp
    PRIVATE
        main.cpp
)

# ---- Include Paths ----------------------------------------------------------
target_include_directories(MyApp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---- Properties -------------------------------------------------------------
set_target_properties(MyApp PROPERTIES
    CXX_EXTENSIONS OFF           # Matches root setting (good practice)
    # OUTPUT_NAME myapp          # Optional: lower-case output name
    # VERSION ${PROJECT_VERSION} # Optional: version info
)

# ---- Compile Features -------------------------------------------------------
# Even though root sets it globally, this makes it explicit
target_compile_features(MyApp
    PRIVATE
        cxx_std_23
)

# ---- Compile Options --------------------------------------------------------
# target_compile_options(MyApp
#     PRIVATE
#         $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
#         $<$<CXX_COMPILER_ID:GCC>:-Wno-sign-compare>
# )

# ---- Dependencies -----------------------------------------------------------
target_link_libraries(MyApp
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
configure_target(MyApp)

message(STATUS "MyApp configured (standalone executable)")
