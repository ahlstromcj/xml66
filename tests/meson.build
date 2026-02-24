#*****************************************************************************
# meson.build (xml66/tests)
#-----------------------------------------------------------------------------
##
# \file        tests/meson.build
# \library     xml66
# \author      Chris Ahlstrom
# \date        2026-02-20
# \updates     2026-02-20
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xml66" library. See the top-level meson.build
#  file for license information.
#
#-----------------------------------------------------------------------------

xml66_tests_exe = executable(
   'xml66_tests',
   sources : [ 'xml66_tests.cpp' ],
   dependencies : [ libxml66_dep, libcfg66_library_dep, liblib66_library_dep ]
   )

test('C++ Xml66 Tests', xml66_tests_exe)
   
#****************************************************************************
# meson.build (xml66/tests)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
