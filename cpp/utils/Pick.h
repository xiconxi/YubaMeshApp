#ifndef PICK_H
#define PICK_H
#include "../controller/RenderController.h"
#include <QReadWriteLock>


class LIBSHARED_EXPORT PickTool: public RenderScript {
public:
    PickTool();

    void meshPick(int x, int y);

    int  getSingleFacePick(int x, int y);

    void createBufferScript();

    void clearPicks();

private:
    void pickScript();

    void sizingScreenBufferScript();

    void drawResult(QTime& t);

    GLuint m_fbo, m_rbo[2];
    std::vector<std::array<int,2> > visible_picks;
    std::vector<std::array<int,2> > screen_coords;
    QReadWriteLock pick_lock;
    friend class RenderCtrl;
};

#endif // PICK_H
