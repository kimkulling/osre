#pragma once

#include <osre/UI/Widget.h>
#include <osre/io/Uri.h>

namespace OSRE {
namespace UI {

class Image : public Widget {
public:
    Image( const String &name, Widget *parent );
    virtual ~Image();
    virtual void setUri( const IO::Uri &imageUri );
    virtual const IO::Uri &getUri() const;

protected:
    void onRender( UiRenderCmdCache &renderCmdCache, RenderBackend::RenderBackendService *rbSrv ) override;

private:
    IO::Uri m_imageUri;
};

}
}
