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
///	@brief  This class is used to create a render-scene.
///
/// Scenes are the container for all content which shall be rendered.
/// A scene can be seen by defining one or more active cameras. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Scene : public Common::Object {
public:
    /// @brief  The class constructor with the name and the requested render-mode.
    /// @param  worldName   [in] The world name.
    /// @param  renderMode  [in] The requested render mode. @see RenderMode
    explicit Scene(const String &worldName);

    /// @brief  The class destructor.
    ~Scene() override = default;

    /// @brief Will add a new entity.
    /// @param entity   The entity to add.
    void addEntity(Entity *entity);
    
    /// @brief Will remove the entity from the world.
    /// @param entity   The entity to remove.
    bool removeEntity(Entity *entity);

    /// @brief Will return the number of entities in the scene.
    /// @return Number of entities.
    size_t getNumEntities() const;

    /// @brief Will return the entity instance at the given index.
    /// @param index    The requested index.
    /// @return The entity instance or nullptr for invalid indices.
    Entity *getEntityAt(size_t index) const;

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
    const cppcore::TArray<Entity *>& getEntityArray() const;

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

inline TransformComponent *Scene::getRootNode() const {
    return mRoot;
}

inline const cppcore::TArray<Entity *> &Scene::getEntityArray() const {
    return mEntities;
}

inline Common::Ids &Scene::getIds() {
    return mIds;
}

} // Namespace App
} // Namespace OSRE

