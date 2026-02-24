#if ! defined XML66_HPP
#define XML66_HPP

/*
 *  This file is part of xml66.
 *
 *  xml66 is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  xml66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with xml66; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          xml66.hpp
 *
 *    This module provides a simple informational function.
 *
 * \library       Any application or library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-02-20
 * \license       GNU GPL v2 or above
 *
 */

#if defined __cplusplus

/*
 * namespace xyz
 *
 *      This module currently defines no namespace.
 */

#include <string>                       /* std::string class                */

/**
 *  Free functions.
 */

extern const std::string & xml66_version () noexcept;

#endif          // defined __cplusplus

#endif          // XML66_HPP

/*
 * xml66.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
