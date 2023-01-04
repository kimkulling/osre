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

#include <osre/App/AppCommon.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Common/Object.h>
#include <osre/Common/Ids.h>
#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace App {

class Entity;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to create a world for the engine. Worlds are the container for all
/// content which shall be rendered.
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
    virtual ~World();

    /// @brief  Will add a new view.
    /// @param  view        [in] A pointer showing to the view.
    Camera *addCamera(const String &name);

    /// @brief  Will set the active view.
    /// @param  activeView  [i] A pointer showing to the active view.
    /// @return true, if set was successful, false if the view is not part of the world.
    Camera *setActiveCamera( Camera *activeView );

    /// @brief  Will set the active view.
    /// @param  viewName    [in] The name of the view.
    /// @return true, if set was successful, false if the view is not part of the world.
    Camera *setActiveCamera( const String &viewName );

    /// @brief  Will return the number of assigned views.
    /// @return The number of assigned views.
    size_t getNumCameras() const;

    /// @brief  Will return the view at the given index.
    /// @param  i       [in] The index.
    /// @return A pointer showing to the view or a nullptr in case of an error.
    Camera *getCameraAt(ui32 i) const;

    /// @brief  Will return the active view.
    /// @return A pointer showing to the active view or nullptr, if no view is currently active.
    Camera *getActiveView() const;

    /// @brief Will add a new entity.
    /// @param entity   The entity to add.
    void addEntity( Entity *entity );
    
    /// @brief Will remove the entity from the world.
    /// @param entity   The entity to remove.
    bool removeEntity(Entity *entity);
    
    /// @brief 
    /// @param name 
    /// @return 
    Entity *getEntityByName( const String &name ) const;

    ///	@brief
    void getEntityArray(cppcore::TArray<Entity *> &entities);

    ///	@brief
    /// @param root 
    void setSceneRoot(Node *root);
    
    /// @brief 
    /// @return 
    Node *getRootNode() const;

    /// @brief  Will update the world.
    /// @param  dt      [in] The current delta time-tick.
    void update( Time dt );

    /// @brief  Will draw the world-
    /// @param  rbService   [in] The renderbackend.
    void draw( RenderBackend::RenderBackendService *rbService );

    /// @brief  Will return the id container.
    /// @return The Id container.    
    const Common::Ids &getIds() const;
    
protected:
    void updateBoundingTrees();

private:
    cppcore::TArray<Camera*> mViews;
    cppcore::THashMap<ui32, Camera*> mLookupViews;
    cppcore::TArray<Entity*> mEntities;
    Camera *mActiveCamera;
    Node *mRoot;
    Common::Ids mIds;
    RenderBackend::Pipeline *mPipeline;
    bool mDirtry;
};

inline size_t World::getNumCameras() const {
    return mViews.size();
}

inline Node *World::getRootNode() const {
    return mRoot;
}

inline void World::getEntityArray(cppcore::TArray<Entity *> &entities) {
    entities = mEntities;
}

inline const Common::Ids &World::getIds() const {
    return mIds;
}

} // Namespace App
} // Namespace OSRE

