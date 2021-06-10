from conans import ConanFile, tools


class F2rConan(ConanFile):
    name = "f2r"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    description = "<Description of F2r here>"
    url = "None"
    license = "None"
    author = "None"
    topics = None
    generators = [ 'pkg_config' ]
    requires = [ 
        'cxxopts/2.2.1',
        'glbinding/3.1.0',
        'libmp3lame/3.100'
    ]

    def system_requirements(self):
        if tools.os_info.is_linux:
            installer = tools.SystemPackageTool()
            installer.install('sdl2', 'libsdl2-dev')
            installer.install('fmt', 'libfmt-dev')
            installer.install('spdlog', 'libspdlog-dev')
        else:
            self.requires.add('sdl2/2.0.15')
            self.requires.add('fmt/7.1.3')
            self.requires.add('spdlog/1.8.5')
            pass

    def package(self):
        pass

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
