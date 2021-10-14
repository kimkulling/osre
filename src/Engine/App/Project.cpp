/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include <osre/App/Project.h>
#include <osre/App/World.h>
#include <osre/Common/Logger.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>

#include <fstream>
#include <sstream>
#include <string>

namespace OSRE {
namespace App {

using namespace ::OSRE::IO;

namespace Details {
static const c8 *Tag = "Project";
static const c8 *EmptyAttributeToken = "empty";

static String buildVersionString(i32 major, i32 minor) {
    std::stringstream stream;

    stream << "v" << major << "." << minor;
    const String version = stream.str();

    return version;
}

static bool parseVersionString(const String &version, i32 &major, i32 &minor) {
    major = minor = -1;
    if (version.empty()) {
        return false;
    }

    const char *ptr = &version[1];
    major = ::atoi(ptr);
    while (*ptr != '.') {
        ++ptr;
    }
    minor = ::atoi(++ptr);

    return true;
}

} // Namespace Details

Project::Project() :
        Object("App/Project"),
        m_version(-1, -1),
        m_flags(-1),
        m_projectName(),
        m_activeWorld(nullptr) {
    // empty
}

Project::~Project() {
    // empty}
}

bool Project::create(const String &name, i32 major, i32 minor) {
    if (isCreated()) {
        return false;
    }

    m_version.mMajor = major;
    m_version.mMinor = minor;
    m_projectName = name;

    return true;
}

bool Project::isCreated() const {
    return (!m_projectName.empty());
}

bool Project::destroy() {
    if (!isCreated()) {
        return false;
    }

    return true;
}

void Project::setProjectName(const String &projectName) {
    if (m_projectName != projectName) {
        m_projectName = projectName;
    }
}

const String &Project::getProjectName() const {
    return m_projectName;
}

void Project::setActiveWorld(World *activeWorld) {
    m_activeWorld = activeWorld;
}

World *Project::getActiveWorld() const {
    return m_activeWorld;
}

i32 Project::getMajorVersion() const {
    return m_version.mMajor;
}

i32 Project::getMinorVersion() const {
    return m_version.mMinor;
}

bool Project::load(const String &name, i32 &major, i32 &minor, i32 flags) {
    if (name.empty()) {
        osre_warn(Details::Tag, "Project name is empty.");
        return false;
    }

    m_projectName = name;
    m_flags = flags;
    String oldPath = Directory::getCurrentDirectory();
    if (!Directory::setCurrentDirectory(name)) {
        return false;
    }

    if (!loadMetadata(major, minor)) {
        Directory::setCurrentDirectory(oldPath);
        return false;
    }

    IO::Uri uri(m_projectName);
    if (uri.isValid()) {
    }

    Directory::setCurrentDirectory(oldPath);

    return true;
}

bool Project::save(const String &name, i32 /*flags*/) {
    if (!isCreated()) {
        return false;
    }

    if (name.empty()) {
        return false;
    }

    String oldPath, path, file;
    oldPath = Directory::getCurrentDirectory();
    Directory::getDirectoryAndFile(name, path, file);
    if (path.empty()) {
        path = file;
        if (m_projectName.empty()) {
            m_projectName = file;
        }
    }

    if (!Directory::exists(path)) {
        if (!Directory::createDirectory(path.c_str())) {
            return false;
        }
    }

    if (!Directory::setCurrentDirectory(path)) {
        return false;
    }


    if (nullptr == m_activeWorld) {
        Directory::setCurrentDirectory(oldPath);
        return true;
    }

    bool res(false);

    IO::Uri uri(name);
    if (uri.isValid()) {
    }
    Directory::setCurrentDirectory(oldPath);

    return res;
}

bool Project::loadMetadata(i32 &major, i32 &minor) {
    return false;
}

bool Project::saveMetadata(i32 major, i32 minor) {
    return false;
}

} // Namespace App
} // Namespace OSRE
