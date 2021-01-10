# ShaderImage
传统的GPUImage都是通过SurfaveView/TextureView贴图来实现，相对ImageView而言显得过重。此实现主要是通过离屏方渲染，得到指定类型的滤镜特效，再更新图片对应bitmap的数据，特别适合列表页等大量使用滤镜的场景。

![image](https://github.com/tanpuer/ShaderImage/blob/master/images/shaderImage.png)
![image](https://github.com/tanpuer/ShaderImage/blob/master/images/show.gif)

优势：
1. 使用的是ImageView，而不是SurfaceView，因而更轻量。
2. 采用精简模式，依赖Android原生解码bitmap，不额外引入任何库。


目前看来还存在的问题：
1. 无法自动释放native的线程，需要手动调用ShaderImageView的release方法。
2. 由于glReadPixels一般只支持rgba和rgb，所以bitmap暂时也只能支持这2种格式，不然就要引入libYUV来进行转换，违背精简的初衷。
