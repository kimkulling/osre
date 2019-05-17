#include <osre/App/Project.h>
#include <osre/Assets/AssetDataArchive.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Directory.h>
#include <osre/Scene/World.h>
#include <osre/Common/Logger.h>

#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>

#include <fstream>

namespace OSRE {
namespace App {

using namespace ::OSRE::Assets;
using namespace ::OSRE::IO;

static const c8* Tag = "Project";

Project::Project()
: Object("App/Project")
, m_project()
, m_activeWorld( nullptr ) {
    // empty
}

Project::~Project() {
    // empty}
}

bool Project::create(const String &name) {
    m_project = name;

    return true;
}

bool Project::load(const String& name, i32 major, i32 minor) {
    if (name.empty()) {
        osre_warn(Tag, "Project name is empty.");
        return false;
    }

    m_project = name;

    loadMetadata();

    IO::Uri uri(m_project);
    if (uri.isValid()) {
        AssetDataArchive importer(major, minor );
        m_activeWorld = importer.load(uri);
    }

    return nullptr != m_activeWorld;
}

bool Project::save(const String &name, i32 major, i32 minor) {
    if (name.empty()) {
        return false;
    }

    String path, file;
    Directory::getDirectoryAndFile(name, path, file);
    if (!Directory::exists(path)) {
        Directory::createDirectory(path.c_str());
    }
    saveMetadata();

    if (nullptr == m_activeWorld) {
        return true;
    }

    bool res(false);

    IO::Uri uri(name);
    if (uri.isValid()) {
        AssetDataArchive exporter(major, minor);
        res = exporter.save(m_activeWorld, uri);
    }

    return res;
}

bool Project::loadMetadata() {
    String metaName = m_project + ".proj";

    std::ofstream file;
    file.open(metaName, std::ofstream::in);
    if (!file.is_open()) {
        return false;
    }

    return true;
}

bool Project::saveMetadata() {
    String metaName = m_project + ".proj";
    Json::Value meta;
    if (nullptr != m_activeWorld) {
        meta["name"] = m_project;
        meta["activeWorld"] = m_activeWorld->getName();
    }

    std::ofstream file;
    file.open(metaName, std::ofstream::out);
    if (!file.is_open()) {
        return false;
    }
     
    Json::StreamWriterBuilder builder;
    Json::StreamWriter* sw = builder.newStreamWriter();
    return 0 == sw->write(meta, &file );
}

}
}
