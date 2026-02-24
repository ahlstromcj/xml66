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
 * \file          xml_tests.cpp
 *
 *      A test-file for the rudimentary CLI parser class and C API.
 *
 * \library       xml66
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-02-24
 * \license       See above.
 *
 *  To do: add a help-line for each option.
 */

#include <cstdlib>                      /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iomanip>                      /* std::setw()                      */
#include <iostream>                     /* std::cout, std::cerr             */
#include <string>                       /* std::string                      */

#include "cli/parser.hpp"               /* cli::parser, etc.                */
#include "xml66.hpp"                    /* xml66_version() function         */
#include "xml/xml66xx.hpp"              /* xml66::XMLnnn classes            */

namespace   // anonymous
{

/*
 * Explanation text.
 */

static const std::string help_intro
{
    "This test program illustrates and tests the C++ interface of the\n"
    "xml66 library.  Options are as follows:\nTODO\n"
};

/*--------------------------------------------------------------------------
 * basic_test_1()
 *--------------------------------------------------------------------------*/

bool
basic_test_1 (bool verbose)
{
    bool result { false };
	std::string testdata_path { "tests/data/RosegardenPatchFile.xml" };
    std::cout
        << "Test 1: Find all banks in " << testdata_path << "."
        << std::endl
        ;

    /*
     * We pass false (the default value of the second parameter because
     * otherwise we get an xml66::XMLException:
     *
     *      validity error : Validation failed: no DTD found !
     */

	xml66::XMLTree doc(testdata_path);  /* do not validate this XML file!   */

	/*
     * "//bank" gives as last element an empty element libxml: bug?
     *
     *      SharedNodeListPtr = std::shared_ptr<XMLSharedNodeList>
     *      XMLSharedNodeList = vector<shared_ptr<XMLNode>>
     */

	xml66::SharedNodeListPtr nodeptrs { doc.find("//bank[@name]") };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " banks." << std::endl;
        result = sz == 8;
        verbose = verbose || sz <= 4;   /* allow short output even if quiet */
        if (result)
        {
            int bankno { 0 };
            for (auto i : *nodeptrs)
            {
                std::string banknam { i->property("name")->value() };
                ++bankno;
                result = i->name() == "bank";
                if (result)
                    result = i->property("name");

                if (verbose)
                {
                    std::cout
                    << "Bank " << bankno << " (" << banknam << "):"
                    << std::endl
                    ;
                }
                if (result)
                {
                    int progno { 0 };
                    for (auto j : i->children())
                    {
                        ++progno;
                        if (verbose)
                        {
                            std::string prognum { j->property("id")->value() };
                            std::string prognam
                            {
                                j->property("name")->value()
                            };
                            std::cout
                                << "   Program " << std::setw(3) << prognum
                                << ": '" << prognam << "'"
                                << std::endl
                                ;
                        }
                    }
                    if (! verbose)
                    {
                        std::cout
                            << "   Bank " << bankno
                            << " (" << banknam << "):"
                            << " has " << progno << " programs."
                            << std::endl
                            ;
                    }
                }
                else
                {
                    std::cerr
                        << "property is not 'name' or name is not 'banks'"
                        << std::endl
                        ;
                    result = false;
                }
            }
        }
        else
        {
            std::cerr << "Incorrect number of banks" << std::endl;
            result = false;
        }
    }
    else
    {
        std::cerr << "Could not create XMLTree" << std::endl;
        result = false;
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_1b(): additional tests over the original test above
 *--------------------------------------------------------------------------*/

bool
basic_test_1b (bool verbose)
{
    bool result { false };
	std::string testdata_path { "tests/data/RosegardenPatchFile.xml" };
    std::cout
        << "Test 1b: Find devices & librarian in " << testdata_path << "."
        << std::endl
        ;

    /*
     * We pass false (the default value of the second parameter because
     * otherwise we get an xml66::XMLException:
     *
     *      validity error : Validation failed: no DTD found !
     */

	xml66::XMLTree doc(testdata_path);  /* do not validate this XML file!   */

	/*
     * "//bank" gives as last element an empty element libxml: bug?
     *
     *      SharedNodeListPtr = std::shared_ptr<XMLSharedNodeList>
     *      XMLSharedNodeList = vector<shared_ptr<XMLNode>>
     */

	xml66::SharedNodeListPtr nodeptrs { doc.find("//device[@name]") };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " devices." << std::endl;
        result = sz == 1;
        verbose = verbose || sz <= 4;   /* allow short output even if quiet */
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                std::string devno { i->property("id")->value() };
                std::string devnam { i->property("name")->value() };
                std::string typenam { i->property("type")->value() };
                if (verbose)
                {
                    std::cout
                        << "    Device " << devno
                        << " '" << devnam << "'"
                        << " is a '" << typenam << "' device."
                        << std::endl
                        ;
                }
            }

            xml66::SharedNodeListPtr libptrs { doc.find("//librarian[@name]") };
            sz = libptrs->size();
            std::cout << "Found " << sz << " librarian." << std::endl;
            for (auto lib : *libptrs)
            {
                xml66::XMLNodePtr node { lib };
                result = bool(lib);
                std::string libnam { node->property("name")->value() };
                std::string email { node->property("email")->value() };
                std::cout
                    << "   Librarian '" << libnam << "' (" << email << ")"
                    << std::endl
                    ;
            }
        }
        else
        {
            std::cerr << "Incorrect number of devices" << std::endl;
            result = false;
        }
    }
    else
    {
        std::cerr << "Could not create XMLTree" << std::endl;
        result = false;
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_1c(): additional tests over the original test above
 *--------------------------------------------------------------------------*/

bool
basic_test_1c (bool verbose)
{
    bool result { false };
	std::string testdata_path { "tests/data/RosegardenPatchFile.xml" };
    std::cout
        << "Test 1c: Find controls & instruments in " << testdata_path << "."
        << std::endl
        ;

    /*
     * We pass false (the default value of the second parameter because
     * otherwise we get an xml66::XMLException:
     *
     *      validity error : Validation failed: no DTD found !
     */

	xml66::XMLTree doc(testdata_path);  /* do not validate this XML file!   */

	/*
     * "//bank" gives as last element an empty element libxml: bug?
     *
     *      SharedNodeListPtr = std::shared_ptr<XMLSharedNodeList>
     *      XMLSharedNodeList = vector<shared_ptr<XMLNode>>
     */

	xml66::SharedNodeListPtr nodeptrs
    {
        doc.find("//controls/control[@name]")
    };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " controls." << std::endl;
        result = sz == 8;
        verbose = verbose || sz <= 8;   /* allow short output even if quiet */
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                std::string ctlnam { i->property("name")->value() };
                std::string typenam { i->property("type")->value() };
                std::string minval { i->property("min")->value() };
                std::string defalt { i->property("default")->value() };
                std::string maxval { i->property("max")->value() };
                if (verbose)
                {
                    std::cout
                        << "   Control " << " '" << ctlnam << "'"
                        << " (" << typenam << ") "
                        << minval << " to " << maxval
                        << " [" << defalt << "]"
                        << std::endl
                        ;
                }
            }

#if 0  // TODO
            xml66::SharedNodeListPtr insptrs { doc.find("//instrument[@name]") };
            sz = insptrs->size();
            std::cout << "Found " << sz << " librarian." << std::endl;
            for (auto lib : *libptrs)
            {
                xml66::XMLNodePtr node { lib };
                result = bool(lib);
                std::string libnam { node->property("name")->value() };
                std::string email { node->property("email")->value() };
                std::cout
                    << "   Librarian '" << libnam << "' (" << email << ")"
                    << std::endl
                    ;
            }
#endif
        }
        else
        {
            std::cerr << "Incorrect number of controls" << std::endl;
            result = false;
        }
    }
    else
    {
        std::cerr << "Could not create XMLTree" << std::endl;
        result = false;
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_2()
 *--------------------------------------------------------------------------*/

bool
basic_test_2 (bool verbose)
{
    bool result { false };
	std::string testdata_path { "tests/data/RosegardenPatchFile.xml" };
    std::cout
        << "Test 2: In " << testdata_path << ",\n"
        << "   find all programs with names containing 'Latin'."
        << std::endl
        ;
	xml66::XMLTree doc(testdata_path);  /* the default is no validation     */
	xml66::SharedNodeListPtr nodeptrs
    {
        doc.find
        (
            "/rosegarden-data/studio/device/bank/"
            "program[contains(@name, 'Latin')]"
        )
    };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " banks." << std::endl;
        result = sz == 5;
        verbose = verbose || sz <= 8;   /* allow short output even if quiet */
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                xml66::XMLNodePtr node { i };
                result = bool(i);
                if (result && verbose)
                {
                    std::string prognum { node->property("id")->value() };
                    std::string prognam { node->property("name")->value() };
                    std::cout
                        << "   Program " << std::setw(3) << prognum
                        << ": '" << prognam << "'"
                        << std::endl
                        ;
                }
            }
        }
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_3()
 *--------------------------------------------------------------------------*/

bool
basic_test_3 (bool verbose)
{
    bool result { false };
    std::string testsession_path { "tests/data/TestSession.ardour" };
    std::cout
        << "Test 3: In " << testsession_path << ",\n"
        << "   find all Sources where captured-for contains 'Guitar'."
        << std::endl
        ;

    xml66::XMLTree doc(testsession_path);
	xml66::SharedNodeListPtr nodeptrs
    {
        doc.find("/Session/Sources/Source[contains(@captured-for, 'Guitar')]")
    };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " sources." << std::endl;
        result = sz == 16;
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                xml66::XMLNodePtr node { i };
                result = bool(i);
                if (result && verbose)
                {
                    std::string srcnum { node->property("id")->value() };
                    std::string srcnam { node->property("name")->value() };
                    std::cout
                        << "   Source " << std::setw(3) << srcnum
                        << ": '" << srcnam << "'"
                        << std::endl
                        ;
                }
            }
        }
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_4()
 *--------------------------------------------------------------------------*/

bool
basic_test_4 (bool verbose)
{
    bool result { false };
    std::string testsession_path { "tests/data/TestSession.ardour" };
    std::cout
        << "Test 4: In " << testsession_path << ",\n"
        << "   find all elments with an 'id' and 'name' attribute."
        << std::endl
        ;

    xml66::XMLTree doc(testsession_path);
	xml66::SharedNodeListPtr nodeptrs { doc.find("//*[@id and @name]") };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " elements." << std::endl;
        result = sz > 0;
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                xml66::XMLNodePtr node { i };
                result = bool(i) && not_nullptr(node->property("id")) &&
                    not_nullptr(node->property("name"));

                if (result && verbose)
                {
                    std::string elemnum { node->property("id")->value() };
                    std::string elemnam { node->property("name")->value() };
                    std::cout
                        << "   Element " << std::setw(5) << elemnum
                        << ": '" << elemnam << "'"
                        << std::endl
                        ;
                }
            }
        }
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_5()
 *--------------------------------------------------------------------------*/

bool
basic_test_5 (bool verbose)
{
    bool result { false };
    std::string testmidnam_path { "tests/data/ProtoolsPatchFile.midnam" };
    std::cout
        << "Test 5: From " << testmidnam_path << ",\n"
        << "   get banks and patches for 'Name Set 1'."
        << std::endl
        ;

    xml66::XMLTree doc(testmidnam_path);
	xml66::SharedNodeListPtr nodeptrs
    {
        doc.find
        (
            "/MIDINameDocument/MasterDeviceNames/ChannelNameSet[@Name="
            "'Name Set 1']/PatchBank"
        )
    };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " items." << std::endl;
        result = sz == 16;
        if (result)
        {
            /*
             * xml66::XMLNodePtr i
             */

            for (auto i : *nodeptrs)
            {
                xml66::XMLProperty * name { i->property("Name") };
                result = not_nullptr(name);
                if (result)
                {
                    if (verbose)
                    {
                        std::cout
                            << "Found Patchbank '" << name->value() << "'"
                            << std::endl
                            ;
                    }

                    xml66::SharedNodeListPtr patchptrs
                    {
                        doc.find("//Patch[@Name]", i.get())
                    };
                    result = not_nullptr(patchptrs);
                    if (result)
                    {
                        for (auto p : *patchptrs)
                        {
                            xml66::XMLProperty * pnum { p->property("Number") };
                            xml66::XMLProperty * pnam { p->property("Name") };
                            result = not_nullptr(pnum) && not_nullptr(pnam);
                            if (result && verbose)
                            {
                                std::cout
                                    << "   Patch "
                                    << std::setw(4) << pnum->value()
                                    << ": '" << pnam->value() << "'"
                                    << std::endl
                                    ;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_6()
 *--------------------------------------------------------------------------*/

bool
basic_test_6 (bool verbose)
{
    bool result { false };
    std::string testmidnam_path { "tests/data/ProtoolsPatchFile.midnam" };
    std::cout
        << "Test 6: In " << testmidnam_path << ", find attribute notes."
        << std::endl
        ;

    xml66::XMLTree doc(testmidnam_path);
	xml66::SharedNodeListPtr nodeptrs { doc.find("//@Value") };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " attributes." << std::endl;
        result = sz > 0;
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                xml66::XMLNodePtr node { i };
                result = bool(i);
                if (result && verbose)
                {
                    std::cout
                        << "   Attribute '" << node->name()
                        << "' = " << node->attribute_value()
                        << std::endl
                        ;
                }
            }
        }
    }
    return result;
}

/*--------------------------------------------------------------------------
 * basic_test_7()
 *--------------------------------------------------------------------------*/

bool
basic_test_7 (bool verbose)
{
    bool result { false };
    std::string testmidnam_path { "tests/data/ProtoolsPatchFile.midnam" };
    std::cout
        << "Test 7: In " << testmidnam_path << ",\n"
        << "   find available channels on 'Name Set 1'."
        << std::endl
        ;

    xml66::XMLTree doc(testmidnam_path);
	xml66::SharedNodeListPtr nodeptrs
    {
        doc.find
        (
		    "//ChannelNameSet[@Name = 'Name Set 1']"
            "//AvailableChannel[@Available = 'true']/@Channel"
        )
    };
    if (not_nullptr(nodeptrs))
    {
        std::size_t sz { nodeptrs->size() };
        std::cout << "Found " << sz << " channels." << std::endl;
        result = sz == 15;
        if (result)
        {
            for (auto i : *nodeptrs)
            {
                xml66::XMLNodePtr node { i };
                result = bool(i);
                if (result && verbose)
                {
                    std::cout
                        << "   Available channel '" << node->name()
                        << "': " << node->attribute_value()
                        << std::endl
                        ;
                }
            }
        }
    }
    return result;
}

}   // namespace anonymous

/*
 * main() routine
 */

int
main (int argc, char * argv [])
{
    cli::parser clip;                   /* provides global/stock options    */
    bool success { clip.parse(argc, argv) };
    std::cout << "Test of " << xml66_version() << ":" << std::endl;
    if (success)
    {
        if (clip.show_information_only())
        {
            std::cout
                << "The --verbose option greatly increases the output."
                << std::endl
                ;
        }
        else
        {
            bool verbose { clip.verbose() };
            success = basic_test_1(verbose);
            if (success)
            {
                success = basic_test_1b(verbose);
                if (success)
                    success = basic_test_1c(verbose);
            }
            if (success)
                success = basic_test_2(verbose);

            if (success)
                success = basic_test_3(verbose);

            if (success)
                success = basic_test_4(verbose);

            if (success)
                success = basic_test_5(verbose);

            if (success)
                success = basic_test_6(verbose);

            if (success)
                success = basic_test_7(verbose);

            if (success)
                std::cout << "xml_tests has succeeded." << std::endl;
            else
                std::cerr << "xml_tests has failed." << std::endl;
        }
    }
    return success ? EXIT_SUCCESS : EXIT_FAILURE ;
}

/*
 * xml_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
