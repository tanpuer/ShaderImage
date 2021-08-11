package com.temple.shaderimage

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.res.ResourcesCompat
import androidx.core.view.children
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import kotlinx.android.synthetic.main.activity_list.*
import kotlinx.android.synthetic.main.item_list.view.*

class ListTestActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_list)
        recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = Adapter()
    }

    inner class Adapter : RecyclerView.Adapter<ImageViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ImageViewHolder {
            return ImageViewHolder(
                LayoutInflater.from(this@ListTestActivity)
                    .inflate(R.layout.item_list, parent, false)
            )
        }

        override fun getItemCount(): Int = 100

        override fun onBindViewHolder(holder: ImageViewHolder, position: Int) {
            //0 - origin
            //1 - gray
            //2 - electric shock
            //3 - two path gaussion blur
            holder.loadImage(position % 4)
        }

    }

    inner class ImageViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {

        fun loadImage(type: Int) {
            val options = BitmapFactory.Options()
            //todo 会导致不能复用bitmap，增大内存抖动，但是如果复用bitmap，在c层读取bitmap内容再修改会有问题。
            //todo 快速滑动的时候没必要进行shader
//            options.inMutable = true
            val bitmap = BitmapFactory.decodeResource(resources, R.mipmap.ic_launcher, options)

//            val bitmap2 = Bitmap.createBitmap(144, 144, Bitmap.Config.ARGB_8888)
//            val canvas = Canvas(bitmap2)
//            canvas.drawARGB(255, 255, 255, 255)
//            val drawable = ResourcesCompat.getDrawable(resources, R.mipmap.ic_launcher, null)
//            drawable?.setBounds(0, 0, canvas.width, canvas.height)
//            drawable?.draw(canvas)

            itemView.shaderImageView.setImageBitmap(bitmap)
//            itemView.shaderImageView.setImageBitmap(bitmap2)
            itemView.shaderImageView.setShaderType(type)
        }

    }
}