from conan.tools.files import copy

from conan import ConanFile


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("fmt/10.0.0")

        self.requires("argparse/2.9")

    def build_requirements(self):
        self.test_requires("catch2/3.3.1")

    def configure(self):
        pass

    def generate(self):
        pass
