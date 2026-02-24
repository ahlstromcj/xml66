#*****************************************************************************
# meson.build (xml66/include)
#-----------------------------------------------------------------------------
##
# \file        include/meson.build
# \library     xml66
# \author      Chris Ahlstrom
# \date        2026-02-20
# \updates     2026-02-20
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xml66" library. See the top-level meson.build
#  file for license information.
#
#  Here we locate the headers. The root of the header directory is 'include'.
#
#-----------------------------------------------------------------------------

libxml66_headers += files(
   'xml66.hpp',
   'utfcpp/utf8.h',
   'utfcpp/utf8/checked.h',
   'utfcpp/utf8/core.h',
   'utfcpp/utf8/cpp11.h',
   'utfcpp/utf8/cpp17.h',
   'utfcpp/utf8/cpp20.h',
   'utfcpp/utf8/unchecked.h',
   'xml/xml66xx.hpp'
   )

configure_file(
   input : 'xml66-config.h.in',
   output : 'xml66-config.h',
   configuration : cdata
   )

#****************************************************************************
# meson.build (xml66/include)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
