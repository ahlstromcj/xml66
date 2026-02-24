/*
 *  This file is part of xml66.
 *
 *  seq24 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  seq24 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with seq24; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * xml++.cc --> xml66xx.cpp
 *
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 *
 * Modified for Ardour and released under the same terms.
 */

/**
 * \file          xml66xx.cpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files..
 *
 * \library       xml66 library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-02-20
 * \version       $Revision$
 *
 */

#include <cstring>
#include <iostream>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "cpp_types.hpp"                /* lib66's CSTR() etc. macros       */
#include "utfcpp/utf8.h"                /* header in the utfcpp directory   */
#include "xml/xml66xx.hpp"              /* ditto, xml66::XML classes        */

xmlChar * xml_version = xmlCharStrdup("1.0");

namespace xml66
{

static XMLNode * readnode (xmlNodePtr);
static void writenode (xmlDocPtr, XMLNode *, xmlNodePtr, int);
static XMLSharedNodeList * find_impl
(
    xmlXPathContext * ctxt,
    const std::string & xpath
);

static XMLNode *
readnode (xmlNodePtr node)
{
    std::string name;
    std::string content;
    xmlNodePtr child;
    if (not_nullptr(node->name))
        name = (const char*)node->name;

    XMLNode * tmp { new XMLNode(name) };
    xmlAttrPtr attr;
    for (attr = node->properties; attr; attr = attr->next)
    {
        content.clear();
        if (attr->children)
            content = (char*)attr->children->content;

        tmp->set_property((const char *)(attr->name), content);
    }
    if (node->content)
        tmp->set_content((char *)(node->content));
    else
        tmp->set_content(std::string());

    for (child = node->children; child; child = child->next)
        tmp->add_child_nocopy(*readnode(child));

    return tmp;
}

static void
writenode (xmlDocPtr doc, XMLNode * n, xmlNodePtr p, int root = 0)
{
    xmlNodePtr node { nullptr };
    if (root)
    {
        node = doc->children =
            xmlNewDocNode(doc, 0, (const xmlChar *) CSTR(n->name()), 0);
    }
    else
    {
        node = xmlNewChild(p, 0, (const xmlChar *) CSTR(n->name()), 0);
    }

    if (n->is_content())
    {
        node->type = XML_TEXT_NODE;
        xmlNodeSetContentLen
        (
            node, (const xmlChar *) CSTR(n->content()), n->content().length()
        );
    }

    const XMLPropertyList & props { n->properties() };
    for (auto propiter : props)
    {
        xmlSetProp
        (
            node, (const xmlChar *) CSTR(propiter->name()),
            (const xmlChar *) CSTR(propiter->value())
        );
    }

    const XMLNodeList & children { n->children() };
    for (auto childiter : children)
    {
        writenode(doc, childiter, node);
    }
}

/**
 * Class: XMLProperty
 *
 *      All functions are defined in the class header.
 */

/**
 * Class: XMLTree
 */

XMLTree::XMLTree (const std::string & fn, bool validate) :
    m_filename  (fn)
{
    read_internal(validate);
}

XMLTree::XMLTree (const XMLTree * from) :
    m_filename      (from->filename()),
    m_root          (new XMLNode(*from->root())),
    m_doc           (xmlCopyDoc(from->m_doc, 1)),
    m_compression   (from->compression())
{
    // no code
}

XMLTree::~XMLTree()
{
    if (not_nullptr(m_root))
        delete m_root;

    if (not_nullptr(m_doc))
        xmlFreeDoc(m_doc);
}

int
XMLTree::set_compression (int c)
{
    if (c > 9)
        c = 9;
    else if (c < 0)
        c = 0;

    m_compression = c;
    return m_compression;
}

bool
XMLTree::read_internal (bool validate)
{
    if (not_nullptr(m_root))
    {
        delete m_root;
        m_root = nullptr;
    }
    if (m_doc)
    {
        xmlFreeDoc(m_doc);
        m_doc = nullptr;
    }

    /*
     * Calling this prevents libxml2 from treating whitespace as active
     * nodes. It needs to be called before we create a parser context.
     */

    xmlKeepBlanksDefault(0);

    xmlParserCtxtPtr ctxt { xmlNewParserCtxt() };   /* new parser context   */
    if (ctxt == NULL)
        return false;

    /*
     * Parse the file, activating the DTD validation option.
     */

    if (validate)
    {
        m_doc = xmlCtxtReadFile
        (
            ctxt, CSTR(m_filename), NULL, XML_PARSE_DTDVALID
        );
    }
    else
    {
        m_doc = xmlCtxtReadFile
        (
            ctxt, CSTR(m_filename), NULL, XML_PARSE_HUGE
        );
    }

    if (m_doc == nullptr)               /* check if parsing succeeded       */
    {
        xmlFreeParserCtxt(ctxt);
        return false;
    }
    else                                /* check if validation succeeded    */
    {
        if (validate && ! ctxt->valid)  // == 0)
        {
            xmlFreeParserCtxt(ctxt);
            throw XMLException("Failed to validate document " + m_filename);
        }
    }
    m_root = readnode(xmlDocGetRootElement(m_doc));
    xmlFreeParserCtxt(ctxt);            /* free up the parser context       */
    return true;
}

bool
XMLTree::read_buffer (char const * buffer, bool to_tree_doc)
{
    m_filename.clear();
    delete m_root;
    m_root = nullptr;

    /*
     * Calling this prevents libxml2 from treating whitespace as active.
     */

    xmlKeepBlanksDefault(0);

    xmlDocPtr doc { xmlParseMemory(buffer, std::strlen(buffer)) };
    if (is_nullptr(doc))
        return false;

    m_root = readnode(xmlDocGetRootElement(doc));
    if (to_tree_doc)
    {
        if (m_doc)
            xmlFreeDoc(m_doc);

        m_doc = doc;
    }
    else
        xmlFreeDoc(doc);

    return true;
}

bool
XMLTree::write () const
{
    // XMLNodeList children;

    xmlKeepBlanksDefault(0);

    xmlDocPtr doc { xmlNewDoc(xml_version) };
    xmlSetDocCompressMode(doc, m_compression);
    writenode(doc, m_root, doc->children, 1);

    int result { xmlSaveFormatFileEnc(CSTR(m_filename), doc, "UTF-8", 1) };

#if defined PLATFORM_DEBUG

    if (result == (-1))
    {
        const xmlError * xerr = xmlGetLastError ();
        if (is_nullptr(xerr))
        {
            std::cerr
                << "unknown XML error during xmlSaveFormatFileEnc()."
                << std::endl
                ;
        }
        else
        {
            std::cerr << "xmlSaveFormatFileEnc: error"
                << " domain: " << xerr->domain
                << " code: " << xerr->code
                << " msg: " << xerr->message
                << std::endl
                ;
        }
    }

#endif

    xmlFreeDoc(doc);
    if (result == (-1))
        return false;

    return true;
}

void
XMLTree::debug (FILE * out) const
{
#if defined XML66_DEBUG_ENABLED

    // XMLNodeList children;

    xmlKeepBlanksDefault(0);

    xmlDocPtr doc = xmlNewDoc(xml_version);
    xmlSetDocCompressMode(doc, m_compression);
    writenode(doc, m_root, doc->children, 1);
    xmlDebugDumpDocument(out, doc);
    xmlFreeDoc(doc);
#else
    (void) out;
#endif
}

const std::string &
XMLTree::write_buffer() const
{
    // XMLNodeList children;

    static std::string result;
    char * ptr;
    int len;

    xmlKeepBlanksDefault(0);

    xmlDocPtr doc = xmlNewDoc(xml_version);
    xmlSetDocCompressMode(doc, m_compression);
    writenode(doc, m_root, doc->children, 1);
    xmlDocDumpMemory(doc, (xmlChar **) &ptr, &len);
    xmlFreeDoc(doc);
    result = ptr;
    free(ptr);
    return result;
}

/**
 * Class: XMLNode
 */

static const int PROPERTY_RESERVE_COUNT { 16 };

XMLNode::XMLNode (const std::string & n) :
    m_name(n)
{
    m_proplist.reserve(PROPERTY_RESERVE_COUNT);
}

XMLNode::XMLNode (const std::string & n, const std::string & c) :
    m_name          (n),
    m_is_content    (true),
    m_content       (c)
{
    m_proplist.reserve(PROPERTY_RESERVE_COUNT);
}

XMLNode::XMLNode (const XMLNode & from)
{
    m_proplist.reserve(PROPERTY_RESERVE_COUNT);
    *this = from;
}

XMLNode &
XMLNode::operator = (const XMLNode & from)
{
    if (this != &from)
    {
        clear_lists ();
        m_name = from.name ();
        set_content(from.content());

        const XMLPropertyList & props { from.properties () };
        for (auto propiter : props)
            set_property(CSTR(propiter->name()), propiter->value());

        const XMLNodeList & nodes { from.children () };
        for (auto childiter : nodes)
            add_child_copy(*childiter);
    }
    return *this;
}

XMLNode::~XMLNode ()
{
    clear_lists();
}

void
XMLNode::clear_lists ()
{
    m_selected_children.clear();
    for (auto curchild : m_children)
        delete curchild;

    m_children.clear ();
    for (auto curprop : m_proplist)
        delete curprop;

    m_proplist.clear();
}

bool
XMLNode::operator == (const XMLNode & other) const
{
    if (is_content() != other.is_content())
        return false;

    if (is_content())
    {
        if (content () != other.content ())
            return false;
    }
    else
    {
        if (name() != other.name())
            return false;
    }

    XMLPropertyList const & other_properties { other.properties() };
    if (m_proplist.size() != other_properties.size())
        return false;

    XMLPropertyConstIterator our_prop_iter { m_proplist.begin() };
    XMLPropertyConstIterator other_prop_iter { other_properties.begin() };
    while (our_prop_iter != m_proplist.end ())
    {
        XMLProperty const * our_prop = *our_prop_iter;
        XMLProperty const * other_prop = *other_prop_iter;
        if
        (
            our_prop->name() != other_prop->name() ||
            our_prop->value() != other_prop->value()
        )
        {
            return false;
        }
        ++our_prop_iter;
        ++other_prop_iter;
    }

    XMLNodeList const & other_children { other.children() };
    if (m_children.size() != other_children.size())
        return false;

    XMLNodeConstIterator our_child_iter { m_children.begin() };
    XMLNodeConstIterator other_child_iter { other_children.begin() };
    while (our_child_iter != m_children.end())
    {
        XMLNode const * our_child { *our_child_iter };
        XMLNode const * other_child { *other_child_iter };
        if (*our_child != *other_child)
            return false;

        ++our_child_iter;
        ++other_child_iter;
    }
    return true;
}

bool
XMLNode::operator != (const XMLNode & other) const
{
    return ! (*this == other);
}

const std::string &
XMLNode::set_content (const std::string & c)
{
    m_is_content = ! c.empty();
    m_content = c;
    return m_content;
}

/**
 *  Return the content of the first content child
 *
 *  '<node>Foo</node>'.
 *
 *  the 'node' is not a content node, but has a child-node 'text'.
 *
 * This method effectively is identical to
 *
 *      return this->child("text")->content()
 */

const std::string &
XMLNode::child_content () const
{
    static const std::string s_empty;
    for (auto n : children())
    {
        if (n->is_content())
            return n->content();
    }
    return s_empty;
}

/**
 *  Returns first child matching name.
 */

XMLNode *
XMLNode::child (const char * name) const
{
    if (not_nullptr(name))
    {
        for (auto cur : m_children)
        {
            if (cur->name() == name)
                return cur;
        }
    }
    return 0;
}

/**
 *  Returns all children matching name.
 */

const XMLNodeList &
XMLNode::children (const std::string & n) const
{
    if (n.empty())
    {
        return m_children;
    }
    else
    {
        m_selected_children.clear();
        for (auto cur : m_children)
        {
            if (cur->name() == n)
                m_selected_children.insert(m_selected_children.end(), cur);
        }
        return m_selected_children;
    }
}

XMLNode *
XMLNode::add_child (const char * n)
{
    return add_child_copy(XMLNode (n));
}

void
XMLNode::add_child_nocopy (XMLNode & n)
{
    m_children.insert(m_children.end(), &n);
}

XMLNode *
XMLNode::add_child_copy (const XMLNode & n)
{
    XMLNode * copy { new XMLNode(n) };
    m_children.insert(m_children.end(), copy);
    return copy;
}

// std::shared_ptr<XMLSharedNodeList>

SharedNodeListPtr
XMLTree::find (const std::string xpath, XMLNode * node) const
{
    xmlXPathContext * ctxt { nullptr };
    xmlDocPtr doc { nullptr };
    if (not_nullptr(node))
    {
        doc = xmlNewDoc(xml_version);
        writenode(doc, node, doc->children, 1);
        ctxt = xmlXPathNewContext(doc);
    }
    else
    {
        ctxt = xmlXPathNewContext(m_doc);
    }

#if 0
    std::shared_ptr<XMLSharedNodeList> result =
        std::shared_ptr<XMLSharedNodeList>(find_impl(ctxt, xpath));
#endif

    SharedNodeListPtr result { SharedNodeListPtr(find_impl(ctxt, xpath)) };
    xmlXPathFreeContext(ctxt);
    if (not_nullptr(doc))
        xmlFreeDoc(doc);

    return result;
}

std::string
XMLNode::attribute_value ()
{
    XMLNodeList children { this->children() };
    if (m_is_content)
    {
        throw XMLException
        (
            "XMLNode: attribute_value failed (is_content) "
            "for requested node: " + name()
        );
    }
    if (children.size() != 1)
    {
        throw XMLException
        (
            "XMLNode: attribute_value failed (children.size != 1) "
            "for requested node: " + name()
        );
    }

    XMLNode * child { *(children.begin()) };
    if (! child->is_content())
    {
        throw XMLException
        (
            "XMLNode: attribute_value failed (!child->is_content()) "
            "for requested node: " + name()
        );
    }
    return child->content();
}

XMLNode *
XMLNode::add_content (const std::string & c)
{
    if (c.empty ())
    {
        /*
         * This would add a "</>" child, leading to invalid XML.
         * Also in XML, empty string content is equivalent to no content.
         */

        return nullptr;
    }
    return add_child_copy(XMLNode(std::string(), c));
}

XMLProperty const *
XMLNode::property (const char * name) const
{
    XMLPropertyConstIterator iter { m_proplist.begin() };
    while (iter != m_proplist.end())
    {
        if ((*iter)->name() == name)
            return *iter;

        ++iter;
    }
    return nullptr;
}

XMLProperty const *
XMLNode::property (const std::string & name) const
{
#if 0
    XMLPropertyConstIterator iter = m_proplist.begin();
    while (iter != m_proplist.end())
    {
        if ((*iter)->name() == name) {
            return *iter;
        }
        ++iter;
    }
    return 0;
#endif

     return property(CSTR(name));
}

#if 0

XMLProperty *
XMLNode::property(const char* name)
{
    XMLPropertyIterator iter = m_proplist.begin();

    while (iter != m_proplist.end()) {
        if ((*iter)->name() == name) {
            return *iter;
        }
        ++iter;
    }
    return 0;
}

XMLProperty *
XMLNode::property(const string& name)
{
    XMLPropertyIterator iter = m_proplist.begin();

    while (iter != m_proplist.end()) {
        if ((*iter)->name() == name) {
            return *iter;
        }
        ++iter;
    }

    return 0;
}

#endif

bool
XMLNode::has_property_with_value
(
    const std::string & name,
    const std::string & value
) const
{
    XMLPropertyConstIterator iter { m_proplist.begin() };
    while (iter != m_proplist.end())
    {
        if ((*iter)->name() == name && (*iter)->value() == value)
            return true;

        ++iter;
    }
    return false;
}

bool
XMLNode::set_property (const char * name, const std::string & value)
{
    XMLPropertyIterator iter { m_proplist.begin() };
#if 0
    std::string const v = PBD::sanitize_utf8 (value);           // PBD
#endif
    std::string v { value };
    std::string tmp;
    utf8::replace_invalid(v.begin(), v.end(), std::back_inserter(tmp));
    v = tmp;
    while (iter != m_proplist.end())
    {
        if ((*iter)->name() == name)
        {
            (*iter)->set_value(v);
            return *iter;
        }
        ++iter;
    }

    XMLProperty * new_property { new XMLProperty(name, v) };
    if (is_nullptr(new_property))
        return 0;

    m_proplist.insert(m_proplist.end(), new_property);
    return new_property;
}

bool
XMLNode::get_property (const char * name, std::string & value) const
{
    XMLProperty const * const prop { property(name) };
    if (is_nullptr(prop))
        return false;

    value = prop->value();
    return true;
}

void
XMLNode::remove_property (const std::string & name)
{
    XMLPropertyIterator iter { m_proplist.begin() };
    while (iter != m_proplist.end())
    {
        if ((*iter)->name() == name)
        {
            XMLProperty * property { *iter };
            m_proplist.erase(iter);
            delete property;
            break;
        }
        ++iter;
    }
}

/**
 *  Remove any property with the given name from this node and its children.
 */

void
XMLNode::remove_property_recursively (const std::string & n)
{
    remove_property(n);
    for (auto i : m_children)
    {
        i->remove_property_recursively(n);
    }
}

void
XMLNode::remove_nodes (const std::string & n)
{
    XMLNodeIterator i { m_children.begin() };
    while (i != m_children.end())
    {
        if ((*i)->name() == n)
            i = m_children.erase (i);
        else
            ++i;
    }
}

void
XMLNode::remove_nodes_and_delete (const std::string & n)
{
    XMLNodeIterator i { m_children.begin() };
    while (i != m_children.end())
    {
        if ((*i)->name() == n)
        {
            delete *i;
            i = m_children.erase (i);
        }
        else
            ++i;
    }
}

void
XMLNode::remove_nodes_and_delete
(
    const std::string & propname,
    const std::string & val
)
{
    XMLNodeIterator i { m_children.begin() };
    while (i != m_children.end())
    {
        XMLProperty const * prop = (*i)->property(propname);
        if (not_nullptr(prop) && prop->value() == val)
        {
            delete *i;
            i = m_children.erase(i);
        }
        else
            ++i;
    }
}

void
XMLNode::remove_node_and_delete
(
    const std::string & n,
    const std::string & propname,
    const std::string & val
)
{
    for (XMLNodeIterator i = m_children.begin(); i != m_children.end(); ++i)
    {
        if ((*i)->name() == n)
        {
            XMLProperty const * prop = (*i)->property (propname);
            if (not_nullptr(prop) && prop->value() == val)
            {
                delete *i;
                m_children.erase(i);
                break;
            }
        }
    }
}

static XMLSharedNodeList *
find_impl (xmlXPathContext * ctxt, const std::string & xpath)
{
    xmlXPathObject * result
    {
        xmlXPathEval((const xmlChar *) CSTR(xpath), ctxt)
    };
    if (is_nullptr(result))
    {
        xmlFreeDoc(ctxt->doc);
        xmlXPathFreeContext(ctxt);
        throw XMLException("Invalid XPath: " + xpath);
    }
    if (result->type != XPATH_NODESET)
    {
        xmlXPathFreeObject(result);
        xmlFreeDoc(ctxt->doc);
        xmlXPathFreeContext(ctxt);
        throw XMLException("Only nodeset result types are supported.");
    }

    xmlNodeSet * nodeset { result->nodesetval };
    XMLSharedNodeList * nodes { new XMLSharedNodeList() };
    if (nodeset)
    {
        for (int i = 0; i < nodeset->nodeNr; ++i)
        {
            XMLNode * node = readnode(nodeset->nodeTab[i]);
            nodes->push_back(std::shared_ptr<XMLNode>(node));
        }
    }
    else
    {
        /* return empty set */
    }
    xmlXPathFreeObject(result);
    return nodes;
}

/**
 *  Dump a node, its properties, and children to a stream.
 */

void
XMLNode::dump (std::ostream & s, const std::string & p) const
{
    if (m_is_content)
    {
        s << p << "  " << content() << "\n";
    }
    else
    {
        s << p << "<" << m_name << ">\n";
        for (auto i : m_children)
        {
            i->dump (s, p + "  ");
        }
        s << p << "</" << m_name << ">\n";
    }
}

}               // namespace xml66

/*
 * xml66xx.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
