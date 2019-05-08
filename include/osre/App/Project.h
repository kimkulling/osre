#pragma once

#include <osre/Common/Object.h>
#include <osre/Scene/SceneCommon.h>

namespace OSRE {
namespace App {
	
class Project : public Common::Object {
public:
	Project( const String &name );
	~Project();
	bool load( const String &name, i32 major, i32 minor );
	bool save( const String &name, i32 major, i32 minor );
    bool loadMetadata();
    bool saveMetadata();

private:
    String m_project;
    Scene::World *m_activeWorld;
};

}
}
