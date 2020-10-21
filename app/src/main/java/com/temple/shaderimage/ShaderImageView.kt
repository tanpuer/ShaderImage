package com.temple.shaderimage

import android.content.Context
import android.graphics.Bitmap
import android.os.Handler
import android.os.Looper
import android.util.AttributeSet
import android.util.Log
import androidx.appcompat.widget.AppCompatImageView

class ShaderImageView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : AppCompatImageView(context, attrs, defStyleAttr) {

    private var mPixelWidth = -1
    private var mPixelHeight = -1
    private var mBitmap: Bitmap? = null
    private val mHandler = Handler(Looper.getMainLooper())

    init {
        onCreate()
    }

    override fun setImageBitmap(bm: Bitmap?) {
        super.setImageBitmap(bm)
        mBitmap = bm
        bm?.let {
            if (mPixelWidth != it.width || mPixelHeight != it.height) {
                mPixelWidth = it.width
                mPixelHeight = it.height
            }
            performShader(mPixelWidth, mPixelHeight)
        }
    }

    fun currentBitmap(index: Int): Bitmap? {
        Log.d("ShaderImageView", "currentBitmap $index")
        return mBitmap
    }

    private external fun onCreate()
    private external fun performShader(width: Int, height: Int)
    private external fun onDestroy()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}