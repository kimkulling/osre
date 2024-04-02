/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Actions/ActionBase.h"

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class Ids;
}

namespace App {
    class World;
    class Entity;
}

namespace Editor {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
///	@brief  This class will wrap the asset import command.
//-------------------------------------------------------------------------------------------------
class ImportAction : public ActionBase {
public:
    /// @brief The class constructor.
    /// @param ids          The id container.
    /// @param activeWorld  The active world to run the command in.
    ImportAction(Common::Ids *ids, App::World *activeWorld);

    /// @brief  The default class destructor.
    ~ImportAction() override = default;

    /// @brief Will return the new entity.
    /// @return The new entity.
    App::Entity *getEntity() const;

    /// @brief Will return the new number of vertices.
    /// @return The new number of imported vertices.
    ui32 getNumVertices() const;

    /// @brief Will return the number of new imported triangles.
    /// @return The number of new imported triangles.
    ui32 getNumTriangles() const;
    
protected:
    bool onRun(const ArgumentList &args) override;

private:
    Common::Ids *mIds;
    App::World *mActiveWorld;
    App::Entity *mEntity;
    ui32 mNumVertices, mNumTriangles;
};

inline App::Entity *ImportAction::getEntity() const {
    return mEntity;
}

inline ui32 ImportAction::getNumVertices() const {
    return mNumVertices;
}

inline ui32 ImportAction::getNumTriangles() const {
    return mNumTriangles;
}

} // namespace Editor
} // namespace OSRE
