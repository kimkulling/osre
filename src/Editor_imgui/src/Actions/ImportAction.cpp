/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Actions/ImportAction.h"
#include <osre/Common/osre_common.h>
#include <osre/App/AssimpWrapper.h>
#include <osre/IO/Uri.h>

#include <cppcore//Common/Variant.h>
#include <osre/Common/Ids.h>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::Common;
using namespace ::OSRE::App;

using namespace ::cppcore;

ImportAction::ImportAction(Ids *ids, World *activeWorld) :
        ActionBase(), 
        mIds(ids),
        mActiveWorld(activeWorld),
        mEntity(nullptr),
        mNumVertices(0u),
        mNumTriangles(0u) {
    // empty
}

bool ImportAction::onRun(const ArgumentList &args) {
    if (args.isEmpty()) {
        return false;
    }
    
    const Variant *modelPath = args[0];
    if (modelPath == nullptr) {
        return false;
    }

    const String path = modelPath->getString();
    if (path.empty()) {
        return false;
    }

    IO::Uri loc(path);
    AssimpWrapper assimpWrapper(*mIds, mActiveWorld);
    if (!assimpWrapper.importAsset(loc, 0)) {
        return false;
    }
    
    assimpWrapper.getStatistics(mNumVertices, mNumTriangles);
    mEntity = assimpWrapper.getEntity();
    
    return (mEntity != nullptr);
}

} // namespace Editor
} // namespace OSRE
