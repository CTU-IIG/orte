/**
 * 
 *	This file is part of shape_android.
 *
 *  shape_android is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  shape_android is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with shape_android.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
package org.ocera.orte.shape_android;

import java.util.ArrayList;
import java.util.Random;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import org.ocera.orte.DomainApp;

/**
 * View, where publisher objects are drawn.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class PublisherView extends View
{
	//TODO maybe different speeds of shapes?
	private static final int STEP_X = 2;
	private static final int STEP_Y = 2;

	public ArrayList<PublisherShape> shapes = new ArrayList<PublisherShape>();
	
	private Random random = new Random();
	
	/**
	 * Just compatibility method.
	 * 
	 * @param context
	 * @param attrs
	 * @since 1.0
	 */
	public PublisherView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}
	
	/**
	 * Update publisher shape's position.
	 * 
	 * @since 1.0
	 */
	public void countShapes()
	{
		for (PublisherShape shape : shapes) {
			if (!shape.getManual()) {
				if (shape.getBounds().left <= 0) shape.setIncX(STEP_X);
				if (shape.getBounds().top <= 0) shape.setIncY(STEP_Y);
				if (shape.getBounds().right >= getWidth()) shape.setIncX(-STEP_X);
				if (shape.getBounds().bottom >= getHeight()) shape.setIncY(-STEP_Y);
			} else {
				shape.setIncX(0);
				shape.setIncY(0);
			}
			
			shape.setBounds(shape.getBounds().left + shape.getIncX(), shape.getBounds().top + shape.getIncY(), shape.getBounds().right + shape.getIncX(), shape.getBounds().bottom + shape.getIncY());
			shape.publication.send(shape.toSend());
		}
	}	
	
	/**
	 * Draw all publisher objects.
	 * 
	 * @since 1.0
	 */
	@Override
	protected void onDraw(Canvas canvas)
	{
		for (PublisherShape shape : shapes) {
			shape.draw(canvas);
		}
	}

	@Override
	public boolean performClick()
	{
		return super.performClick();
	}
	
	/**
	 * When touch the publisher object, start manual
	 * mode and let user change position of object. When
	 * user finishes, just generate random direction and
	 * let object moving automatically again.
	 * 
	 * @since 1.0
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int myX = 0, myY = 0;
		
		switch (event.getActionMasked()) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
			myX = (int) event.getX(event.getActionIndex());
			myY = (int) event.getY(event.getActionIndex());
			
			for (PublisherShape shape : shapes) {
				if (myX <= shape.getBounds().right
						&& myX >= shape.getBounds().left
						&& myY >= shape.getBounds().top
						&& myY <= shape.getBounds().bottom) {
					shape.setManual(true);
					shape.setPointerId(event.getPointerId(event.getActionIndex()));
					break;
				}
			}
			
			return true;
		case MotionEvent.ACTION_MOVE:
			for (PublisherShape shape : shapes) {
				if (shape.getManual()) {
					myX = (int) event.getX(event.findPointerIndex(shape.getPointerId()));
					myY = (int) event.getY(event.findPointerIndex(shape.getPointerId()));

					shape.setBounds((int) (myX - shape.getShape().getWidth()/2), (int) (myY - shape.getShape().getHeight()/2), (int) (myX + shape.getShape().getWidth()/2), (int) (myY + shape.getShape().getHeight()/2));
				}
			}
			
			return true;
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
			myX = (int) event.getX(event.getActionIndex());
			myY = (int) event.getY(event.getActionIndex());
			
			for (PublisherShape shape : shapes) {
				if (myX <= shape.getBounds().right
						&& myX >= shape.getBounds().left
						&& myY >= shape.getBounds().top
						&& myY <= shape.getBounds().bottom) {
					shape.setManual(false);
					shape.setIncX(STEP_X * (1 -2*random.nextInt(2)));
					shape.setIncY(STEP_Y * (1 -2*random.nextInt(2)));
				}
			}
			
			return true;
		default:
			return super.onTouchEvent(event) || this.performClick();
		}
	}
	
	/**
	 * When OK is selected on {@link NewDialogFragment},
	 * this method is called to add new publisher object.
	 *  
	 * @param color
	 * @param appDomain
	 * @since 1.0
	 */
	public void addShape(int color, DomainApp appDomain)
	{
		this.shapes.add(new PublisherShape(random.nextInt(3), color, appDomain));
	}
}
