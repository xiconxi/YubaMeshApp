# 线框(wireframe)
这里所说的wireframe特指通过Geometry shader与Fragment shader配合完成的反走样线框.具体的原理不做展开,[OpenGL 4 Shading Language Cookbook, 2nd]几何着色器一章中有详细介绍.如下图

![image](https://github.com/xiconxi/YubaMs/raw/master/screenshots/20180603-223426.png)

基于shader的线框其实现基础为三角形插值.
## 矩形线框

利用三角形插值还可以实现四边形线框,一种实现方式如下图,假定所有三角形图元均是直角三角形,那么几何着色器中只输出直角边即可实现与矩形更为接近的四边形线框.适用范围为四边形网格或按斜边相邻的直角三角形组成的三角网格.

![image](https://github.com/xiconxi/YubaMs/raw/master/screenshots/20181104-230554.png)

## 四边形线框

对于任意的三角网格,其每一三角形图元内心到三个顶点的线可组成一个四边形线框,进一步可以给出确定的定义: 线框所在的像素到其中一条边的距离小于内接圆半径,并且到剩余两条边的距离相等.具体的效果如下图所示:

![image](https://github.com/xiconxi/YubaMs/raw/master/screenshots/20181105-001340.png)

## 多边形线框

对于上一个线框模式,考虑到相反的模式,我们该如何画出三角形内心到三条边的线,从而组成线框.对于内心而言,其实质是角平分线,不满足要求,进一步使用边的平分线即中线,我们画出中线交点到各边的线,即可.具体效果如下图所示:

![image](https://github.com/xiconxi/YubaMs/raw/master/screenshots/20181105-074353.png)


