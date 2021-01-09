package com.temple.shaderimage

import android.content.Intent
import android.graphics.Bitmap
import android.graphics.Canvas
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.core.content.res.ResourcesCompat
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val bitmap = Bitmap.createBitmap(144, 144, Bitmap.Config.ARGB_8888)
//        val bitmap = Bitmap.createBitmap(144, 144, Bitmap.Config.ARGB_4444)

        //todo gl那边glReadPixels暂时只支持rgb和rgba，如果有其他类型，需要用libyuv去转换格式，因为此实现为精简模式，就不做这部分了
//        val bitmap = Bitmap.createBitmap(144, 144, Bitmap.Config.RGB_565)
//        val bitmap = Bitmap.createBitmap(144, 144, Bitmap.Config.ALPHA_8)
        val canvas = Canvas(bitmap)
        canvas.drawARGB(255, 255, 255, 255)
        val drawable = ResourcesCompat.getDrawable(resources, R.mipmap.ic_launcher, null)
        drawable?.setBounds(0, 0, canvas.width, canvas.height)
        drawable?.draw(canvas)
        shaderImageView.setImageBitmap(bitmap)
        shaderImageView.setShaderType(3)

        button.setOnClickListener {
            startActivity(Intent(this, ListTestActivity::class.java))
        }
    }
}
