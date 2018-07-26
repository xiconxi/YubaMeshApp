# YubaMs (OSX)

基于QtQuick/OpenGL(4.2)的插件化网格测量:


基本

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/yuba.png)

3D拾取

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/3d_pick.png)

3D拾取(osx)
hints:
1. mac ratine: devicePixelRatio == 2
2. qtquick(may bug): Item's size is the same as the parent's, even use anchors
3. 
![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/yuba_osx.png)

网格选区(osx)

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/face_select.png)

效果图：
多纹理合并：
    输入为多张纹理，混合系数以面元为单位，即各个点拥有纹理张数个数的纹理坐标，各个面元拥有纹理张数个数的混合系数

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/20180623-191330.png)

wireframe

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/20180603-223426.png)

周期纹理

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/20180603-190305.png)

正面真实纹理、背面周期纹理(纹理坐标作为周期的坐标轴)

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/20180603-155820.png)

周期纹理表示，以显示纹理密度

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/20180603-154820.png)

扫描动画

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/scan_animation.gif)

选取动画

![image](https://github.com/xiconxi/YubaMs/raw/2018/screenshots/pick_select_animation.gif)

