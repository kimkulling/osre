/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "SamplesAppBase.h"

namespace OSRE {

using namespace ::OSRE::Common;
using namespace ::OSRE::App;

CameraComponent *SampleAppBase::setupCamera(const String &name, Scene *scene, ui32 w, ui32 h, Ids &ids) {
    auto *camEntity = new Entity(name, ids, scene);
    scene->addEntity(camEntity);
    auto *camera = dynamic_cast<CameraComponent*>(camEntity->createComponent(ComponentType::CameraComponentType));
    scene->setActiveCamera(camera);
    camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

    return camera;
}

}