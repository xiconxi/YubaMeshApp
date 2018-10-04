#ifndef SCRIPTSAMPLES_H
#define SCRIPTSAMPLES_H

#include "../controller/RenderController.h"
namespace YbCore {
class defaultScript{
public:
    static void clear(QTime& t);
// test
    static void test1(QTime& t);
    static RenderFunc test2;
    static RenderFunc nullRender;
};
typedef defaultScript Script;
}
#endif // SCRIPTSAMPLES_H
