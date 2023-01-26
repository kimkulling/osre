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
#pragma once

#include <osre/Common/Object.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    class RenderBackendService;
}

namespace App {

class World;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Stage : public Common::Object {
public:
    /// @brief 
    /// @param stageName 
    Stage(const String &stageName);
    
    /// @brief 
    ~Stage() override;
    
    /// @brief 
    /// @param name 
    /// @return 
    World *createWorld(const String &name);
    
    /// @brief 
    /// @param name 
    /// @return 
    World *findWorld(const String &name) const;
    
    /// @brief 
    /// @return 
    size_t getNumberOfWorlds() const;
    
    /// @brief 
    /// @param index 
    /// @return 
    World *getWorldAt(ui32 index) const;
    
    /// @brief 
    /// @param name 
    /// @return 
    bool setActiveWorld(const String &name);
    
    /// @brief 
    /// @param world 
    void setActiveWorld(World * world);
    
    /// @brief 
    /// @return 
    World *getActiveWorld() const;
    
    /// @brief 
    /// @return 
    bool isEmpty() const {
        return mWorld == nullptr;
    }

    /// @brief  Will update the world.
    /// @param  dt      [in] The current delta time-tick.
    void update(Time dt);

    /// @brief  Will draw the world-
    /// @param  rbService   [in] The renderbackend.
    void draw(RenderBackend::RenderBackendService *rbService);

private:
    World *mWorld;
    cppcore::TArray<World*> mWorlds;
};

} // namespace App
} // namespace OSRE
