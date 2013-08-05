package org.ocera.orte.demo;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Paint.Style;
import android.util.AttributeSet;
import android.view.View;

public class HokuyoView extends View {
	
	public static final double HOKUYO_START_ANGLE = 239.77/2;
	public static final int HOKUYO_SPLIT_DIVISION = 1024;
	public static final int HOKUYO_ORIENTATION = 1;
	public static final double HOKUYO_RANGE_ANGLE_LEFT = 70.0;
	public static final double HOKUYO_RANGE_ANGLE_RIGHT = 70.0;

	private int[] data = new int[681];
	private Paint paint = new Paint();
	private Path path = new Path();
	private boolean isRunning = false;
	private boolean hasBeenDrawn = true;

	public HokuyoView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		if (isRunning) {
			paint.setStyle(Style.STROKE);
			paint.setStrokeWidth(3);
			paint.setColor(Color.BLACK);
			paint.setAntiAlias(true);
	
			path.reset();
			path.moveTo(getWidth()/2, getHeight());
			for(int i = 0; i < data.length; i++) {
				double ang = HOKUYO_INDEX_TO_RAD(i);
		        if((ang<(-HOKUYO_RANGE_ANGLE_LEFT/180.0*Math.PI))||((ang>(HOKUYO_RANGE_ANGLE_RIGHT/180.0*Math.PI)))) {
	                continue;
		        }
	
		        data[i] /= 10;
	            int x = (int)(getWidth()/2) - (int)(data[i] * Math.sin(HOKUYO_INDEX_TO_RAD(i)));
	            int y = getHeight() -(int)(data[i] * Math.cos(HOKUYO_INDEX_TO_RAD(i)));
				path.lineTo(x, y);
			}
			path.close();
			
			canvas.drawPath(path, paint);
		}
		else
			canvas.drawARGB(0, 0, 0, 0);
		hasBeenDrawn = true;
	}
	
	public void run(boolean run) {
		isRunning = run;
	}
	
	public void setData(int[] data) {
		if (hasBeenDrawn) {
			this.data = data.clone();
			hasBeenDrawn = false;
			postInvalidate();
		}
	}
	
	public static double HOKUYO_INDEX_TO_DEG(int index) {
		return ((HOKUYO_START_ANGLE-index*360.0/HOKUYO_SPLIT_DIVISION) * HOKUYO_ORIENTATION);
	}
	
	public static double HOKUYO_INDEX_TO_RAD(int index) {
		return (HOKUYO_INDEX_TO_DEG(index)/180.0*Math.PI);
	}
}
