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
#include "Actions/ExportAction.h"
#include "Common/osre_common.h"
#include "App/AssimpWrapper.h"
#include "IO/Uri.h"
#include "Common/Ids.h"


#include <assimp/Exporter.hpp>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::Common;
using namespace cppcore;

ExportAction::ExportAction(Ids *ids, const aiScene *activeScene) : mIds(ids), mActiveScene(activeScene) {
    // empty
}

bool ExportAction::onRun(const ArgumentList &args) {
    if (args.isEmpty() || args.size() < 2) {
        return false;
    }

    if (mActiveScene == nullptr) {
        return true;
    }

    const Variant *format = args[0];
    if (format == nullptr) {
        return false;
    }
    const String strFormat = format->getString();
    if (strFormat.empty()) {
        return false;
    }

    const Variant *modelPath = args[1];
    if (modelPath == nullptr) {
        return false;
    }

    const String path = modelPath->getString();
    if (path.empty()) {
        return false;
    }

    Assimp::Exporter exporter;
    const aiReturn ret = exporter.Export(mActiveScene, strFormat, path);
    return ret == aiReturn_SUCCESS;
}

}
}