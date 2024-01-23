#include "ViewingPlane.h"
#include "Volume.h"

VOLUME_TEMPLATE
class Renderer {
public:
    virtual void render() = 0;
    virtual void set_volume(const Volume<length> * volume) = 0;
    virtual void set_view(const ViewingPlane * view) = 0;
    virtual void set_light_pos(const vec3 * pos) = 0;
};