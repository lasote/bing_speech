from conans import ConanFile, CMake, tools


class BingspeechConan(ConanFile):
    name = "bing_speech"
    version = "0.1"
    license = "MIT"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Bingspeech here>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    requires = ("easy_sound/0.1@lasote/testing",
                "libcurl/7.56.1@bincrafters/stable",
                "rapidjson/1.1.0@bincrafters/stable")
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["bing_speech"]

