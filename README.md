# ShaderImage
传统的GPUImage都是通过SurfaveView/TextureView贴图来实现，相对ImageView而言显得过重。此实现主要是通过离屏方渲染，得到指定类型的滤镜特效，再更新图片对应bitmap的数据，特别适合列表页等大量使用滤镜的场景。
![image](https://github.com/tanpuer/ShaderImage/blob/master/images/show.gif)
