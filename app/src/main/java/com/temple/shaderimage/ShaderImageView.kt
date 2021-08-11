package com.temple.shaderimage

import android.content.Context
import android.graphics.Bitmap
import android.util.AttributeSet
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.AppCompatImageView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleObserver
import androidx.lifecycle.OnLifecycleEvent

class ShaderImageView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : AppCompatImageView(context, attrs, defStyleAttr), LifecycleObserver {

    private var mPixelWidth = -1
    private var mPixelHeight = -1
    private var mBitmap: Bitmap? = null

    init {
        onCreate()
        (context as? AppCompatActivity)?.lifecycle?.addObserver(this)
    }

    override fun setImageBitmap(bm: Bitmap?) {
        super.setImageBitmap(bm)
        mBitmap = bm
    }

    fun setShaderType(type: Int) {
        mBitmap?.let {
            if (mPixelWidth != it.width || mPixelHeight != it.height) {
                mPixelWidth = it.width
                mPixelHeight = it.height
            }
            performShader(mPixelWidth, mPixelHeight, this, type)
        }
    }

    /**
     * called from C++
     */
    fun currentBitmap(): Bitmap? {
        return mBitmap
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_DESTROY)
    private fun onActivityDestroy() {
        Log.d(TAG, "onActivityDestroyed")
        (context as? AppCompatActivity)?.lifecycle?.removeObserver(this)
        onDestroy()
    }

    private external fun onCreate()

    private external fun performShader(
        width: Int,
        height: Int,
        shaderImageView: ShaderImageView,
        shaderType: Int
    )

    private external fun onDestroy()

    companion object {
        init {
            System.loadLibrary("shaderImage")
        }

        private const val TAG = "ShaderImageView"
    }

}