from conan import ConanFile

class MyProjectConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeConfigDeps"

    def requirements(self):
        self.requires("fmt/12.1.0")
        self.requires("opengl/system")
        self.requires("glew/2.2.0")
        self.requires("glfw/3.4")
