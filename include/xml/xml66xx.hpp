#if ! defined XML66_XML_XML66XX_HPP
#define XML66_XML_XML66XX_HPP

/*
 *  This file is part of cfg66.
 *
 *  cfg66 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  cfg66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with cfg66; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * xml++.h --> xml66xx.hpp
 *
 * Copyright (C) 2006-2016 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2006 Taybin Rutkin <taybin@taybin.com>
 * Copyright (C) 2008-2009 David Robillard <d@drobilla.net>
 * Copyright (C) 2008 Hans Baier <hansfbaier@googlemail.com>
 * Copyright (C) 2009-2011 Carl Hetherington <carl@carlh.net>
 * Copyright (C) 2015-2017 Robin Gareus <robin@gareus.org>
 * Copyright (C) 2016 Tim Mayberry <mojofunk@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * xml++.h
 *
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 * Modified for Ardour and released under the same terms.
 */

/**
 * \file          xml66xxp.hpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files..
 *
 * \library       xml66 library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-02-23
 * \version       $Revision$
 *
 */

#include <cstdarg>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>                      /* std::as_const()                  */
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "c_macros.h"                   /* lib66's is_nullptr() etc. macros */
#include "util/strconversions.hpp"      /* util::to_string<> templates      */

namespace xml66
{

class XMLTree;
class XMLNode;

/**
 * XMLProperty
 */

class XMLProperty
{

private:

    std::string m_name { };
    std::string m_value { };

public:

    XMLProperty () = delete;

    XMLProperty
    (
        const std::string & n,
        const std::string & v = ""
    ) :
        m_name  (n),
        m_value (v)
    {
        // No code
    }

    ~XMLProperty () = default;

    const std::string & name () const
    {
        return m_name;
    }

    const std::string & value () const
    {
        return m_value;
    }

    const std::string & set_value (const std::string & v)
    {
        return m_value = v;
    }

};          // class XMLProperty

/**
 *  Type aliases.
 */

using XMLNodePtr                = std::shared_ptr<XMLNode>;
using XMLNodeList               = std::vector<XMLNode *>;
using XMLSharedNodeList         = std::vector<XMLNodePtr>;
using SharedNodeListPtr         = std::shared_ptr<XMLSharedNodeList>;
using XMLNodeIterator           = XMLNodeList::iterator;
using XMLNodeConstIterator      = XMLNodeList::const_iterator;
using XMLPropertyList           = std::vector<XMLProperty *>;
using XMLPropertyIterator       = XMLPropertyList::iterator;
using XMLPropertyConstIterator  = XMLPropertyList::const_iterator;

/**
 * XMLTree
 */

class XMLTree
{

private:

    std::string m_filename { };
    XMLNode *   m_root { nullptr };
    xmlDocPtr   m_doc { nullptr };
    int         m_compression { 0 };

public:

    XMLTree () = default;
    XMLTree (const std::string & fn, bool validate = false);
    XMLTree (const XMLTree *);
    ~XMLTree ();

    XMLNode * root () const
    {
        return m_root;
    }

    XMLNode * set_root (XMLNode * n)
    {
        return m_root = n;
    }

    const std::string & filename () const
    {
        return m_filename;
    }

    const std::string & set_filename (const std::string& fn)
    {
        return m_filename = fn;
    }

    int compression () const
    {
        return m_compression;
    }

    int set_compression (int);

    bool read ()
    {
        return read_internal(false);
    }

    bool read (const std::string & fn)
    {
        set_filename(fn);
        return read_internal(false);
    }

    bool read_and_validate ()
    {
        return read_internal(true);
    }

    bool read_and_validate (const std::string & fn)
    {
        set_filename(fn);
        return read_internal(true);
    }

    bool read_buffer (char const *, bool to_tree_doc = false);
    bool write () const;

    bool write (const std::string & fn)
    {
        set_filename(fn);
        return write();
    }

    void debug (FILE *) const;
    const std::string & write_buffer () const;

    // TODO use alias

    SharedNodeListPtr find          // std::shared_ptr<XMLSharedNodeList>
    (
        const std::string xpath, XMLNode * = nullptr
    ) const;

private:

    bool read_internal (bool validate);

};          // class XMLTree

/**
 * XMLNode
 */

class XMLNode
{

private:

    std::string         m_name { };
    bool                m_is_content { false };
    std::string         m_content { };
    XMLNodeList         m_children { };
    XMLPropertyList     m_proplist { };
    mutable XMLNodeList m_selected_children { };

public:

    XMLNode () = delete;
    XMLNode (const std::string & name);
    XMLNode (const std::string & name, const std::string & content);
    XMLNode (const XMLNode & other);                // TODO &&
    XMLNode & operator = (const XMLNode & other);   // TODO &&
    ~XMLNode ();

    bool operator == (const XMLNode & other) const;
    bool operator != (const XMLNode & other) const;

    const std::string & name () const
    {
        return m_name;
    }

    bool is_content () const
    {
        return m_is_content;
    }

    const std::string & content () const
    {
        return m_content;
    }

    const std::string & set_content (const std::string &);
    XMLNode * add_content (const std::string & s = "");

    const std::string & child_content() const;
    const XMLNodeList & children (const std::string & str = "") const;
    XMLNode * child (const char *) const;
    XMLNode * add_child (const char *);
    XMLNode * add_child_copy (const XMLNode &);
    void add_child_nocopy (XMLNode &);

    std::string attribute_value ();  // throws XMLException if it doesn't exist

    const XMLPropertyList & properties () const
    {
        return m_proplist;
    }

    XMLProperty const * property (const char *) const;
    XMLProperty const * property (const std::string &) const;

    /*
     * C++17 required for this trick.
     */

    XMLProperty * property (const char * cp)
    {
        return const_cast<XMLProperty *>(std::as_const(*this).property(cp));
    }

    XMLProperty * property (const std::string & c)
    {
        return const_cast<XMLProperty *>(std::as_const(*this).property(c));
    }

    bool has_property_with_value
    (
        const std::string &, const std::string &
    ) const;
    bool set_property (const char * name, const std::string & value);

    bool set_property (const char * name, const char * cstr)
    {
        return set_property(name, std::string(cstr));
    }

#if 0
    // NEEDED ???
    bool set_property (const char * name, const Glib::ustring & ustr)
    {
        return set_property(name, ustr.raw());
    }
#endif

    template<class T>
    bool set_property (const char * name, const T & value)
    {
        std::string str;
        if (! util::to_string<T> (value, str))              /* PBD  */
        {
            return false;
        }
        return set_property(name, str);
    }

    bool get_property (const char * name, std::string & value) const;

    template <class T>
    bool get_property (const char * name, T & value) const
    {
        XMLProperty const * const prop { property(name) };
        if (is_nullptr(prop))
            return false;

        return util::string_to<T> (prop->value (), value);  /* PBD  */
    }

    void remove_property (const std::string &);
    void remove_property_recursively (const std::string &);

    /*
     * Remove all nodes with the name passed to remove_nodes.
     */

    void remove_nodes (const std::string &);

    /*
     * Remove and delete all nodes with the name passed to remove_nodes.
     */

    void remove_nodes_and_delete (const std::string &);

    /*
     * Remove and delete all nodes with property propname matching value.
     */

    void remove_nodes_and_delete
    (
        const std::string & propname, const std::string & val
    );

    /*
     * Remove and delete first node with given name and propname
     * matching value.
     */

    void remove_node_and_delete
    (
        const std::string & n,
        const std::string & propname,
        const std::string & val
    );

    void dump (std::ostream &, const std::string & p = "") const;

private:

    void clear_lists ();

};          // class XMLNode

/**
 *  XMLException
 */

class XMLException: public std::exception
{

private:

    std::string m_message { };

public:

    explicit XMLException(const std::string msg) : m_message(msg)
    {
        // no code
    }
    virtual ~XMLException() throw ()
    {
        // no code
    }

    virtual const char * what() const throw()
    {
        return m_message.c_str();
    }

};

}               // namespace xml66

#endif          // XML66_XML_XML66XX_HPP

/*
 * xml66xx.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
