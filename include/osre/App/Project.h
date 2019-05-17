#pragma once

#include <osre/Common/Object.h>
#include <osre/Scene/SceneCommon.h>

namespace OSRE {
namespace App {
	
class OSRE_EXPORT Project : public Common::Object {
public:
	Project();
	~Project();
    bool create(const String &name);
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
