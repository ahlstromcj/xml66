#*****************************************************************************
# meson.build (xml66)
#-----------------------------------------------------------------------------
##
# \file        meson.build
# \library     xml66
# \author      Chris Ahlstrom
# \date        2026-02-20
# \updates     2026-02-23
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xml66" library. It was part of the libs66
#  collection of libraries, but is now separate and stands alone.
#
#  The "xml66" library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  The "xml66" library is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU General Public License along with
#  the "xml66" library. If not, see <https://www.gnu.org/licenses/>.
#
#     'warning_level=everything' is way too much.
#
#-----------------------------------------------------------------------------

project('xml66',
   [ 'c', 'cpp' ],
   version : '0.1',
   license : 'GPLv3',
   meson_version : '>=1.1.0',
   default_options : [
      'c_std=c11', 'cpp_std=c++17', 'warning_level=3',
      'default_library=both'
      ]
   )

#-----------------------------------------------------------------------------
# Conditional enabling of the Potext translation library.
#-----------------------------------------------------------------------------

xml66_use_potext = false
if get_option('potext')

   xml66_use_potext = true
   add_project_arguments('-DXML66_USE_POTEXT', language : [ 'c', 'cpp' ])

endif

#-----------------------------------------------------------------------------
# Information for this sub-project.
#-----------------------------------------------------------------------------
#
#  Tricky: the *.pc file must have a base name matching the name of the
#  subproject *.wrap file.
#
#-----------------------------------------------------------------------------

xml66_pkg_description = 'Refactors base XML configuration/utility modules.'
xml66_pkg_version = meson.project_version()
xml66_api_version = '0.1'
xml66_info_date = '2026-02-20'
xml66_info_build_type = get_option('buildtype')
xml66_info_build_root = meson.project_build_root()
xml66_info_lib_type = get_option('default_library')
xml66_info_project_root = meson.project_source_root()
xml66_debug = get_option('buildtype').contains('debug')
xml66_project_base = 'xml66-@0@'.format(xml66_api_version)
xml66_pkgconfig_name = 'libxml66'
xml66_subproj = meson.is_subproject()

# -fno-inline-functions does not work.

if xml66_debug
   if not xml66_subproj
      add_project_arguments(
         [ '-DDEBUG', '-fno-inline-functions' ],
         language : 'cpp'
         )
   endif
endif

#-----------------------------------------------------------------------------
# Easy access to directory options. Commented out the unnecessary ones.
# Many of these definitions are simply informative.
#-----------------------------------------------------------------------------

xml66_dir = 'xml66-@0@'.format(xml66_api_version)
xml66_prefix = get_option('prefix')
xml66_bindir = join_paths(xml66_prefix, get_option('bindir'))
xml66_includedir = join_paths(xml66_prefix, get_option('includedir'), xml66_dir)
xml66_libdir = join_paths(xml66_prefix, get_option('libdir'), xml66_dir)
xml66_datadir = join_paths(xml66_prefix, get_option('datadir'), xml66_dir)
xml66_docdir = join_paths(xml66_prefix, xml66_datadir, 'doc', xml66_dir)
libraries_top = meson.project_source_root()
libxml66_include_top = join_paths(libraries_top, 'include')

#-----------------------------------------------------------------------------
# The *.pc file goes into $libdir/xml66-0.x/pkgconfig instead of
# $libdir/pkgconfig as the meson documentation asserts.  Not sure why, but
# wiring the expected directory here works.
#-----------------------------------------------------------------------------

alt_pkgconfig_libdir = join_paths(
   xml66_prefix, get_option('libdir'), 'pkgconfig'
   )

#-----------------------------------------------------------------------------
# libxml66_headers provides the list of all headers in the project. These are
# currently all installable, public headers. They are set in
# include/meson.build.
#
# libxml66_sources provides the source-code files. They are set in
# src/meson.build.
#-----------------------------------------------------------------------------

libxml66_headers = []
libxml66_sources = []
build_args = []

#-----------------------------------------------------------------------------
# Each include file directory is added to the list passed to the compiler.
# We prefer to use the style '#include "cfg/header.hpp".
#-----------------------------------------------------------------------------

libxml66_includes = include_directories('.',    # libxml66_include_top
   'include',
   'include/utfcpp',
   'include/xml'
   )

#-----------------------------------------------------------------------------
# Used by the xml66_version() function.
#-----------------------------------------------------------------------------

build_args += [
   '-DXML66_NAME=' + '"' + meson.project_name() + '"',
   '-DXML66_VERSION=' + '"' + meson.project_version() + '"'
   ]

#-----------------------------------------------------------------------------
# Only make public interfaces visible
#-----------------------------------------------------------------------------
#
# if target_machine.system() == 'windows' or target_machine.system() == 'cygwin'
#    build_args += '-DLIBPUBLIC="__declspec(dllexport)"'
# else
#    build_args += '-DLIBPUBLIC=__attribute__((visibility("default")))'
# endif

#-----------------------------------------------------------------------------
# Set up some "HAVE" macros.  By default the header file is generated in the
# "build" directory. If configure_file() is moved to include/meson.build,
# the the header goes into "build/include", which is what we want.
#-----------------------------------------------------------------------------

cc = meson.get_compiler('cpp')
cdata = configuration_data()
cdata.set10('limits_h', cc.has_header('limits.h'))

#-----------------------------------------------------------------------------
# Potential sub-projects
#-----------------------------------------------------------------------------

if xml66_use_potext

   libpotext_dep = subproject('libpotext').get_variable('libpotext_dep')
   libpotext_library_dep = dependency(
      'libpotext', fallback : [ 'libpotext', 'libpotext_dep' ]
      )

endif

liblib66_dep = subproject('liblib66').get_variable('liblib66_dep')
liblib66_library_dep = dependency(
   'liblib66', fallback : [ 'liblib66', 'liblib66_dep' ]
   )

libcfg66_dep = subproject('libcfg66').get_variable('libcfg66_dep')
libcfg66_library_dep = dependency(
   'libcfg66', fallback : [ 'libcfg66', 'libcfg66_dep' ]
   )

#-----------------------------------------------------------------------------
# Dependencies when building the libxml66 library.
#
# xmlxx_dep (created in libxml++/meson.build):
#
#   Dependencies when using the libxml++ library.
#-----------------------------------------------------------------------------

libxml2_min_ver = '2.9.0'
libxml2_req = '>= @0@'.format(libxml2_min_ver)
libxml2_dep = dependency(
   ['libxml-2.0', 'LibXml2'],
   version: libxml2_req,
   fallback: ['libxml2']
   )

'''
libxml2_dep = dependency(
   'libxml2', version : '>=2.9.0', required : true
   )
'''

system_depends = [ libxml2_dep ]

xmlxx_pc_requires = []
libxml2_lib_pkgconfig = []

#-----------------------------------------------------------------------------
# Put libxml-2.0 in the 'Requires:' section in the generated pkg-config file if
# we found it by pkg-config. Then make sure we link to libxml-2.0.
#-----------------------------------------------------------------------------

if libxml2_dep.type_name() == 'pkgconfig' or libxml2_dep.type_name() == 'internal'
   xmlxx_pc_requires += ['libxml-2.0' + libxml2_req]
else
   libxml2_lib_pkgconfig += [libxml2_dep.get_variable(
      cmake:         'LIBXML2_LIBRARIES',
      default_value: 'LibXml2.lib',
  )]
endif

#-----------------------------------------------------------------------------
# Sub-directories. 'tests' has to be descended into later.
#-----------------------------------------------------------------------------

subdir('include')
subdir('src')

#-----------------------------------------------------------------------------
# Dependencies on Linux
#-----------------------------------------------------------------------------
#
# None, yet.
#
#-----------------------------------------------------------------------------

empty_depends = [ ]

#-----------------------------------------------------------------------------
# We recommemd using a recent version of meson by installing it outside the
# Linux distro's repository.  For example, meson on Ubuntu is pretty damn
# old.
#
# The choice of library type is of some import for debugging:
#
#     library:          Defaults to a shared library. The User setting is
#                       'default_library'.  See project() above.
#     shared_library:   Cannot see source files until stepped into.
#     static_library:   Can see the source files when debugging.
#
# This one causes the linker to not see the library functions, wotta pain:
#
#     gnu_symbol_visibility: 'hidden'
#
# Restore it later. Also see the https://gcc.gnu.org/wiki/Visibility site.
#
#-----------------------------------------------------------------------------

if xml66_use_potext

   xml66_library_build = library(
      xml66_project_base,
      libxml66_sources,
      install : not xml66_subproj,
      install_dir : xml66_libdir,
      c_args : build_args,
      cpp_args : build_args,
      dependencies : [
         system_depends, liblib66_library_dep, libcfg66_library_dep,
         libpotext_library_dep
         ],
      include_directories : [ libxml66_includes ]
      )

else

   xml66_library_build = library(
      xml66_project_base,
      libxml66_sources,
      install : not xml66_subproj,
      install_dir : xml66_libdir,
      c_args : build_args,
      cpp_args : build_args,
      dependencies : [
         system_depends, liblib66_library_dep, libcfg66_library_dep
         ],
      include_directories : [ libxml66_includes ]
      )

endif

#-----------------------------------------------------------------------------
# Make this library usable as a Meson subproject.  This allows a subproject to
# easily specify how it should be used. This makes it interchangeable with the
# same dependency that is provided externally (i.e. installed) by the system.
#-----------------------------------------------------------------------------

libxml66_dep = declare_dependency(
   include_directories : [ libxml66_includes ],
   link_with : [ xml66_library_build ],
   dependencies : [ libxml2_dep ]
   )

#-----------------------------------------------------------------------------
# Make this library usable from the system's package manager.
#-----------------------------------------------------------------------------

install_headers(libxml66_headers, subdir : xml66_dir, preserve_path : true)

#-----------------------------------------------------------------------------
# Package config
#-----------------------------------------------------------------------------
#  On Arch Linux, the .pc file is installed to /usr/local/lib/pkgconfig.
#  On Ubuntu, the .pc file is installed to
#     /usr/local/lib/x86_64-linux-gnu/pkgconfig.
#-----------------------------------------------------------------------------

if not xml66_subproj

   pkg_mod = import('pkgconfig')
   pkg_mod.generate(
      xml66_library_build,
      filebase : xml66_pkgconfig_name,
      version : xml66_pkg_version,
      name : xml66_project_base,
      description : xml66_pkg_description,
      install_dir : alt_pkgconfig_libdir,
      subdirs : xml66_dir,
      libraries : xml66_library_build,
      url : 'https://github.com/ahlstromcj/xml66',
      )

# TODO: figure out where to put and install the logo

   install_data(
      files(
         'ChangeLog',
         'LICENSE.md',
         'README.md',
         'doc/xml66-library-guide.pdf'
         ),
      install_dir : get_option('docdir')
      )

   if get_option('enable-tests')
      subdir('tests')
   endif

endif

#-----------------------------------------------------------------------------
# Info
#-----------------------------------------------------------------------------

summary(
   {
      'Date' : xml66_info_date,
      'Filebase' : xml66_project_base,
      'Version' : xml66_pkg_version,
      'Base' : xml66_project_base,
      'Description' : xml66_pkg_description,
      'Source Root' : xml66_info_project_root,
      'Headers Root' : libxml66_include_top,
      'Sub Dir' : xml66_project_base,
      'Build Type' : xml66_info_build_type,
      'Build Root' : xml66_info_build_root,
      'Library Type' : xml66_info_lib_type,
      'Subproject' : xml66_subproj
   },
   section : 'Xml66 Project'
   )

summary(
   {
      'prefix' : xml66_prefix,
      'includedir' : xml66_includedir,
      'bindir' : xml66_bindir,
      'libdir' : xml66_libdir,
      'datadir' : xml66_datadir,
      'docdir' : xml66_docdir
   },
   section : 'Xml66 Install Directories'
   )

#****************************************************************************
# meson.build (xml66)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
