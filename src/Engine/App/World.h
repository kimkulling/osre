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

#include "App/AppCommon.h"

#include "Common/Object.h"
#include "Common/Ids.h"

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace App {

// Forward declarations ---------------------------------------------------------------------------
class Entity;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to create the world for the engine. 
///
/// Worlds are the container for all content which shall be rendered.
/// A world is build by set a stage. A stage is the container for scene nodes ( @see Stage ). 
/// A world can be seen by defining an active view. A view represents the camera for looking into 
/// the stage ( @see View ).
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT World : public Common::Object {
public:
    /// @brief  The class constructor with the name and the requested render-mode.
    /// @param  worldName   [in] The world name.
    /// @param  renderMode  [in] The requested render mode. @see RenderMode
    explicit World(const String &worldName);

    /// @brief  The class destructor.
    ~World() override = default;

    /// @brief Will add a new entity.
    /// @param entity   The entity to add.
    void addEntity(Entity *entity);
    
    /// @brief Will remove the entity from the world.
    /// @param entity   The entity to remove.
    bool removeEntity(Entity *entity);

    /// @brief Will search, if the entity is already part of the world instance.
    /// @param[in] name  The entity name to look for.
    /// @return A pointer showing ot the entity or nullptr, if nothing was found.
    Entity *findEntity(const String &name);

    /// @brief Will set the new active camera.
    /// @param[in] camera   The new camera.
    /// @return true if successful, false in case of an error.
    bool setActiveCamera(CameraComponent *camera);
    
    /// @brief 
    /// @param name 
    /// @return 
    Entity *getEntityByName( const String &name ) const;

    ///	@brief
    void getEntityArray(cppcore::TArray<Entity *> &entities);

    ///	@brief
    /// @param[in] root 
    void setSceneRoot(TransformComponent *root);
    
    /// @brief  Will return the root node of the world.
    /// @return Pointer showing to the root node.
    TransformComponent *getRootNode() const;

    /// @brief  
    void init();

    /// @brief  Will update the world.
    /// @param[in] dt  The current delta time-tick.
    void update( Time dt );

    /// @brief  Will render the world-
    /// @param[in] rbService  The renderbackend.
    void render( RenderBackend::RenderBackendService *rbService );

    /// @brief  Will return the id container.
    /// @return The Id container.    
    Common::Ids &getIds();
    
protected:
    /// @brief Will update the whole bounding boxc hierarchy.
    void updateBoundingTrees();

private:
    cppcore::TArray<Entity*> mEntities;
    CameraComponent *mActiveCamera;
    TransformComponent *mRoot;
    Common::Ids mIds;
    RenderBackend::Pipeline *mPipeline;
    bool mDirtry;
};

inline TransformComponent *World::getRootNode() const {
    return mRoot;
}

inline void World::getEntityArray(cppcore::TArray<Entity *> &entities) {
    entities = mEntities;
}

inline Common::Ids &World::getIds() {
    return mIds;
}

} // Namespace App
} // Namespace OSRE

