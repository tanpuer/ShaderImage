package com.temple.shaderimage;

import android.content.Context;
import android.util.AttributeSet;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.AppCompatImageView;

public abstract class JavaImageView extends AppCompatImageView {

    public JavaImageView(@NonNull Context context) {
        super(context);
    }

    public JavaImageView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public JavaImageView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
    }

}
