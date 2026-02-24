# README for Library Xml66 0.1 2026-02-20

__Xml66__ is a small XML support library for some of the "66" projects.
It is meant primarily to create a MIDINAM implemented based on
heavily modified code from the Ardour and UtfCpp projects.

Support sites (still in progress):

    *   https://ahlstromcj.github.io/
    *   https://github.com/ahlstromcj/ahlstromcj.github.io/wiki

# Major Features

The "xml66" directory holds code for handling various tasks needed for
the use of XML. Xml66 contains the following subdirectories, each of which holds modules
or classes in a namespace of the same name:

    *   utfcpp  Contains header-only code to assist in sanitizing UTF-8
                encoded data.
                configuration, including application names, settings basics,
                and an INI-style configuration-file system. Added are
                data-type indicators and help text. Also provided are
                history<> and memento<> templates to support undo/redo.
    *   xml:    Provides C/C++ code to handle parsing and generation.
    *   tests:  Small test applications to test and illustrate
                most of the classes.

    Note that a work.sh script is provided to simplify or clarify various
    operations such as cleaning, building, making a release, and installing
    or uninstalling the library.

##  Library Features

    *   Can be built using GNU C++ or Clang C++.
    *   Basic dependencies: Meson 1.1 and above; C++17 and above;
        libxml2.
    *   The build system is Meson, and sample wrap files are provided
        for using Xml66 as a C++ subproject.
    *   PDF documentation built from LaTeX.

##  Code

    *   The code is a mix of hard-core C++ and C-like functions.
    *   The C++ STL and advanced language features are used as much as
        possible
    *   C++17 is required for some of its features.
    *   The GNU and Clang C++ compilers are supported.
    *   Broken into modules for easier maintenance.

##  Fixes

    *   Tbd.

##  Documentation

    *   A PDF developers guide will eventually be written.

## To Do

    *   Beef up testing.
    *   Beef up the LaTeX documentation.

## Recent Changes

    For all changes, see the NEWS file.

    *   Version 0.1:
        *   We begin!

// vim: sw=4 ts=4 wm=2 et ft=markdown
