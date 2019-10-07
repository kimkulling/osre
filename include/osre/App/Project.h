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

#include <osre/Common/Object.h>
#include <osre/Scene/SceneCommon.h>

namespace Json {
    class StreamWriter;
}

namespace OSRE {
namespace App {
	
class OSRE_EXPORT Project : public Common::Object {
public:
	Project();
	~Project();
    bool create(const String &name, i32 major, i32 minor);
    bool isCreated() const;
    bool destroy();
    void setProjectName(const String& projectName);
    const String& getProjectName() const;
    void setActiveWorld(Scene::World* activeWorld);
    Scene::World* getActiveWorld() const;
    i32 getMajorVersion() const;
    i32 getMinorVersion() const;
    bool load(const String& name, i32& major, i32& minor, i32 flags);
	bool save( const String &name, i32 flags);

protected:
    bool loadMetadata(i32& major, i32& minor);
    bool saveMetadata(i32 major, i32 minor, Json::StreamWriter *streamWriter);

private:
    Version m_version;
    i32 m_flags;
    String m_projectName;
    Scene::World *m_activeWorld;
};

} // Namespace App
} // Namespace OSRE
