#ifndef SLICERENDER_H
#define SLICERENDER_H

#include <YbCore/controller>
#include <glm/vec2.hpp>
class QTime;
class PluginBackend;
class SliceRender : public RenderScript
{
public:
    SliceRender();

    void updateDis(float value);

    void updatePercent(float percent);

    float dis = 0;

    glm::vec2 bounding_z;

    float percent = 0;

private:
    void scanLineAnimation(QTime& t);

    void drawModelWithSlice(QTime &t);

    friend class PluginBackend;
};

#endif // SLICERENDER_H
