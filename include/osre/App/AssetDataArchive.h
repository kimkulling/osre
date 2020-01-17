/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>

namespace Json {
    class StreamWriter;
    class Value;
}

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
    class Uri;
}

namespace RenderBackend {
    struct BufferData;
}

namespace Scene {
    class World;
    class Stage;
    class Node;
    class View;
}

namespace Assets {
    
class OSRE_EXPORT AssetDataArchive {
public:
    AssetDataArchive(i32 majorVersion = CurrentMajorVersion, i32 minorVersion = CurrentMinorVersion);
    ~AssetDataArchive();
    Scene::World *load( const IO::Uri & fileLocation );
    bool save( Scene::World *world, const IO::Uri & fileLocation );
    bool saveStage( Scene::Stage *stage, Json::Value &parent, Json::StreamWriter *sw, std::ofstream &stream);
    bool saveView(Scene::View *view, Json::Value &parent, Json::StreamWriter *sw, std::ofstream &stream);
    void traverseChildren(Scene::Node *currentNode, Json::StreamWriter *sw, std::ofstream &stream);

private:
    bool parseType(Scene::World *world);

private:
    Version mVersion;
};

} // Namespace Assets
} // Namespace OSRE

