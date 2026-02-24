/*
 *  This file is part of xml66.
 *
 *  xml66 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  xml66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with xml66; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          xml66.cpp
 *
 *  This module defines an informational function.
 *
 * \library       xml66
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-02-20
 * \license       GNU GPLv2 or above
 *
 */

#include "xml66.hpp"                   /* no-namespace function library    */

/*
 * Version information string.
 */

const std::string &
xml66_version () noexcept
{
    static std::string s_info { XML66_NAME "-" XML66_VERSION " " __DATE__ };
    return s_info;
}

/*
 * xml66.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
