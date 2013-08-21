package org.ocera.orte.demo;

import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

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
	public static final int HOKUYO_INDEX_LOWER = HOKUYO_DEG_TO_INDEX(HOKUYO_RANGE_ANGLE_LEFT);
	public static final int HOKUYO_INDEX_UPPER = HOKUYO_DEG_TO_INDEX(-HOKUYO_RANGE_ANGLE_RIGHT);
	public static final double COSINUS = Math.cos((90-HOKUYO_RANGE_ANGLE_LEFT)/180.0*Math.PI);

	private int[] data = new int[681];
	private double[] speedCo = new double[2];
	private Paint paint = new Paint();
	private Path path = new Path();
	
	private boolean isRunning = false;
	private boolean isMonitoring = false;
	private boolean hasBeenDrawn = true;
	
	private final ReentrantLock lock = new ReentrantLock();
	private final ReentrantLock lockMotion = new ReentrantLock();
	private final ReentrantReadWriteLock controlRrwl = new ReentrantReadWriteLock(true);
	private final ReadLock rcLock = controlRrwl.readLock();
	private final WriteLock wcLock = controlRrwl.writeLock();

	public HokuyoView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		paint.setStyle(Style.STROKE);
		paint.setStrokeWidth(4);
		paint.setColor(Color.BLACK);
		paint.setAntiAlias(false);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		rcLock.lock();
		try {
			if (isRunning) {
				lock.lock();
				try {
					double norm = (double)getWidth()/(2*COSINUS);
					if (norm > getHeight())
						norm = getHeight();
					canvas.drawLine((int)(getWidth()*0.95),
									(int)(getHeight()*0.97),
									(int)(getWidth()*0.95-norm/4),
									(int)(getHeight()*0.97),
									paint);
					paint.setStrokeWidth(2);
					canvas.drawText("1 m", (int)(getWidth()*0.95-norm/8), (int)(getHeight()*0.97-10), paint);
					paint.setStrokeWidth(3);
					if (!hasBeenDrawn) {
						path.reset();
						path.moveTo(getWidth()/2, getHeight());
						for(int i = HOKUYO_INDEX_LOWER+1; i <= HOKUYO_INDEX_UPPER; i++) {
							data[i] = (int)(((double)data[i]/4000)*norm);
				            int x = (int)(getWidth()/2) - (int)(data[i] * Math.sin(HOKUYO_INDEX_TO_RAD(i)));
				            int y = getHeight() - (int)(data[i] * Math.cos(HOKUYO_INDEX_TO_RAD(i)));
							path.lineTo(x, y);
						}
						path.close();
						hasBeenDrawn = true;
					}
				}
				finally {
					lock.unlock();
				}
			}
			else {
				path.reset();
			}
			canvas.drawPath(path, paint);
			
			if (isMonitoring) {
				lockMotion.lock();
				try {
					double norm;
					if (getHeight() < getWidth())
						norm = getHeight()*0.125;
					else
						 norm = getWidth()*0.125;
					paint.setStrokeWidth(1);
					canvas.drawLine((int)(10),
									(int)(10+norm*1.5),
									(int)(10+norm*3),
									(int)(10+norm*1.5),
									paint);
					canvas.drawLine((int)(10+norm*1.5),
									(int)(10),
									(int)(10+norm*1.5),
									(int)(10+norm*3),
									paint);
					paint.setStrokeWidth(4);
					canvas.drawLine((int)(10+norm*1.5),
									(int)(10+norm*1.5),
									(int)(speedCo[0]*norm+10+norm*1.5),
									(int)(speedCo[1]*norm+10+norm*1.5),
									paint);
				} finally {
					lockMotion.unlock();
				}
			}
		} finally {
			rcLock.unlock();
		}
	}
	
	private void calculateCoordinates(short[] speed) {
		double x, y;
		double temp[] = new double[2];
		
		temp[0] = (double)speed[0]/16000;
		temp[1] = (double)speed[1]/16000;
		
		y = (temp[0]+temp[1])/2;
		x = (temp[0]-y)/0.30;

		speedCo[1] = -y;
		speedCo[0] = speedCo[1]>0 ? -x : x;
	}
	
	public void run(boolean run) {
		wcLock.lock();
		try {
			isRunning = run;
		} finally {
			wcLock.unlock();
		}
	}
	
	public void runMotion(boolean run) {
		wcLock.lock();
		try {
			isMonitoring = run;
		} finally {
			wcLock.unlock();
		}
	}
	
	public void setData(int[] data) {
		if (lock.tryLock()) {
			try {
				this.data = data.clone();
				hasBeenDrawn = false;
			}
			finally {
				lock.unlock();
			}
			postInvalidate();
		}
	}
	
	public void setDataMotion(short[] speed) {
		if (lockMotion.tryLock()) {
			try {
				short[] speedCl = speed.clone();
				calculateCoordinates(speedCl);
			}
			finally {
				lockMotion.unlock();
			}
			postInvalidate();
		}
	}
	
	public static double HOKUYO_INDEX_TO_DEG(int index) {
		return ((HOKUYO_START_ANGLE-index*360.0/HOKUYO_SPLIT_DIVISION) * HOKUYO_ORIENTATION);
	}
	
	public static double HOKUYO_INDEX_TO_RAD(int index) {
		return (HOKUYO_INDEX_TO_DEG(index)/180.0*Math.PI);
	}
	
	public static int HOKUYO_DEG_TO_INDEX(double d) {
		return (int)((HOKUYO_START_ANGLE-(d)/HOKUYO_ORIENTATION)/(360.0/HOKUYO_SPLIT_DIVISION));
	}
}
