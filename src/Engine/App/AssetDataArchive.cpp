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
#include <osre/App/AssetDataArchive.h>
#include <osre/App/World.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Scene/Camera.h>
#include <osre/Scene/Node.h>
#include <osre/App/Component.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Stream.h>
#include <osre/IO/IOService.h>
#include <osre/Common/Logger.h>

#include <json/json.h>
#include <json/reader.h>

#include <fstream>

namespace OSRE {
namespace Assets {

using namespace ::OSRE::IO;
using namespace ::OSRE::Scene;
using namespace ::OSRE::App;

static const c8 *Tag = "AssetData";

namespace Token {
    static const c8 *Type = "type";
    static const c8 *Name = "Name";
    static const c8 *StageAttrib_ActiveState = "activeStage";
}

AssetDataArchive::AssetDataArchive( i32 majorVersion, i32 minorVersion )
: mVersion(majorVersion, minorVersion ){
    // empty
}

AssetDataArchive::~AssetDataArchive() {
    // empty 
}

World *AssetDataArchive::load( const IO::Uri &fileLocation ) {
    if (fileLocation.isEmpty()) {
	    osre_debug( Tag, "File location is empty.");
        return nullptr;
    }

    IO::Stream *file = IOService::getInstance()->openStream(fileLocation, Stream::AccessMode::ReadAccess);
    if (nullptr == file) {
        return nullptr;
    }
    
    const size_t size = file->getSize();
    std::string doc;
    doc.resize(size);
    file->read(&doc[0], size);
    file->close();

    Json::Value content;
    Json::Reader reader;
    if (!reader.parse(doc, content, false)) {
        return nullptr;
    }
    
    if (content.empty()) {
        return nullptr;
    }

    String worldName(fileLocation.getResource());
    World *world = new World( worldName );
    std::string key;
    for (Json::ValueConstIterator it = content.begin(); it != content.end(); ++it) {
        key = it.key().asString();
    }

    return world;
}

static bool writeNode(Scene::Node *currentNode, Json::StreamWriter *sw ) {
    if (nullptr == currentNode || nullptr == sw ) {
        return false;
    }

    Json::Value value;
    value["name"] = currentNode->getName();
    value["type"] = "scene::node";

    return true;
}

bool AssetDataArchive::save( App::World *world, const IO::Uri &fileLocation ) {
    if ( nullptr == world ) {
        return false;
    }

    Json::StreamWriterBuilder builder;
    Json::StreamWriter *sw = builder.newStreamWriter();
    std::ofstream stream(fileLocation.getAbsPath(), std::ofstream::out);
    if (!stream.is_open()) {
        return false;
    }

    String name( world->getName() );
    if (name.empty()) {
        name = "world_1";
    }
    Json::Value worldObj;
    worldObj["type"] = "scene::world";;
    worldObj["name"] = name;

    for (ui32 i = 0; i < world->getNumCameras(); ++i) {
        Camera *currentView(world->getCameraAt(i));
        if (nullptr != currentView) {
            saveCamera(currentView, worldObj, sw, stream);
        }
    }
    Camera *activeView = world->getActiveView();
    if (nullptr != activeView) {
        worldObj["activeView"] = activeView->getName();
    }

    sw->write(worldObj, &stream);
    stream << std::endl;

    return true;
}

bool AssetDataArchive::saveCamera(Scene::Camera *view, Json::Value &parent, Json::StreamWriter *sw, std::ofstream &stream) {
    String name(view->getName());
    if (name.empty()) {
        name = "view_1";
    }

    Json::Value viewObj;
    viewObj["type"] = "scene::view";
    viewObj["name"] = name;

    parent["activeView"] = viewObj;
    sw->write(parent, &stream);

    return true;
}

void AssetDataArchive::traverseChildren( Node *currentNode, Json::StreamWriter *sw, std::ofstream &stream) {
    if ( nullptr == currentNode || nullptr == sw ) {
        return;
    }

    // Write the node
    writeNode( currentNode, sw );

    // Loop over all children
    Node *currentChild( nullptr );
    for ( ui32 i = 0; i < currentNode->getNumChildren(); ++i ) {
        currentChild = currentNode->getChildAt( i );
        if ( nullptr == currentChild ) {
            continue;
        }

        writeNode( currentNode, sw );
        traverseChildren( currentChild, sw, stream );
    }
}

bool AssetDataArchive::parseType( App::World *world ) {
    if (nullptr == world) {
        return false;
    }

    return true;
}

} // Namespace Assets
} // Namespace OSRE
