project(
    'file_trash_tester',
    'c',
    version: '1.0.0'
)

#
# C Stuff
#

c_args = [
    '-fstack-protector',
    '-pedantic',
    '-Wstrict-prototypes',
    '-Wundef',
    '-Werror-implicit-function-declaration',
    '-Wformat',
    '-Wformat-security',
    '-Werror=format-security',
    '-Wconversion',
    '-Wunused-variable',
    '-Wunreachable-code',
    '-Wall',
    '-W'
]

includeDir = include_directories('/usr/include')

#
# Dependencies
#

dep_glib = dependency('glib-2.0', version: '>= 2.46.0')
dep_gio = dependency('gio-2.0', version: '>= 2.46.0')

#
# Global stuff
#

prefix = get_option('prefix')

#
# Create executable
#

executable(
    meson.project_name(),
    [
        'src/utils.c',
        'src/trash_store.c',
        'src/trash_item.c',
        'src/main.c'
    ],
    dependencies: [
        dep_glib,
        dep_gio
    ],
    install: true
)
