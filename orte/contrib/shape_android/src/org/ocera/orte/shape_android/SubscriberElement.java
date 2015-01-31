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

import android.graphics.Canvas;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.graphics.drawable.shapes.PathShape;
import android.graphics.drawable.shapes.RectShape;
import android.view.View;

import org.ocera.orte.DomainApp;
import org.ocera.orte.Subscription;
import org.ocera.orte.SubscriptionCallback;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.RecvInfo;
import org.ocera.orte.types.SubsProp;
import org.ocera.orte.types.ORTEConstant;

/**
 * Subscribing objects are made by this class.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class SubscriberElement extends SubscriptionCallback
{
	private Subscription subscription;
	
	private NtpTime deadline;
	private NtpTime minSeparation;
	
	private BoxType box;
	private ShapeDrawable shape;
	
	private View parentView;
	
	private boolean receiving;
	private boolean enabled;

	/**
	 * Set default variables of subscriber's object.
	 * 
	 * @param appDomain
	 * @param color Topic is dependent on color.
	 * @since 1.0
	 */
	public SubscriberElement(DomainApp appDomain, int color, View v) {
		this.deadline = new NtpTime(6);
		this.minSeparation = new NtpTime(0);
		
		this.box = new BoxType(appDomain, PublisherShape.getColorName(color));
		SubsProp subscriberProperties = new SubsProp(this.box.getTopic(),
				"BoxType",
				this.minSeparation,
				this.deadline,
				ORTEConstant.IMMEDIATE,
				ORTEConstant.BEST_EFFORTS,
				0);
		
		this.subscription = appDomain.createSubscription(subscriberProperties, this.box, this);
		
		this.shape = new ShapeDrawable();
		this.shape.getPaint().setColor(PublisherShape.getColorConstant(color));
		
		this.setShape();
		
		this.parentView = v;
		
		this.enabled = true;
	}
	
	/**
	 * What should be done when new data are received.
	 * 
	 * @param info
	 * @param msg
	 * @since 1.0
	 */
	public void callback(RecvInfo info, MessageData msg)
	{
		switch (info.getRecvStatus()) {
		case ORTEConstant.NEW_DATA:
			this.receiving = true;
			//Log.d("SubscriberElement", "new data: " + (int) ((Box) msg).strength + "(was "+ this.box.strength +"); " + ((Box) msg).rectangle.top_left_x + ", " + ((Box) msg).rectangle.top_left_y + ", "  +((Box) msg).rectangle.bottom_right_x + ", " + ((Box) msg).rectangle.bottom_right_y);
			
			this.shape.getBounds().left = this.box.rectangle.top_left_x;
			this.shape.getBounds().top = this.box.rectangle.top_left_y;
			this.shape.getBounds().right = this.box.rectangle.bottom_right_x;
			this.shape.getBounds().bottom = this.box.rectangle.bottom_right_y;
			
			this.setShape();

			this.parentView.postInvalidate();
			break;
		case ORTEConstant.DEADLINE:
			this.receiving = false;
			this.parentView.postInvalidate();
			break;
		}
	}
	
	/**
	 * Draw shape, which is private object.
	 * 
	 * @param canvas
	 * @since 1.0
	 */
	public void drawMe(Canvas canvas)
	{
		this.shape.draw(canvas);
	}
	
	/**
	 * Set desired shape of object.
	 * 
	 * @since 1.0
	 */
	public void setShape()
	{
		switch (this.box.shape) {
		case 0:
			this.shape.setShape(new RectShape());
			break;
		case 1:
			this.shape.setShape(new OvalShape());
			break;
		case 2:
			this.shape.setShape(new PathShape(PublisherShape.returnTrianglePath(), 96, 96));
			break;
		default:
			this.shape.setShape(new RectShape());
		}
	}

	/**
	 * Getter for enabled value. Should be object
	 * drawn or not?
	 * 
	 * @return Enabled?
	 * @since 1.0
	 */
	public boolean getEnabled()	{ return this.enabled; }
	
	/**
	 * Set value enabled. Should be object
	 * drawn or not?
	 * 
	 * @param b Enable?
	 * @since 1.0
	 */
	public void setEnabled(boolean b) { this.enabled = b; }

	/**
	 * Getter for enabled value. Should be object
	 * drawn or not?
	 * 
	 * @return Enabled?
	 * @since 1.0
	 */
	public boolean getAllowScaling()	{ return this.box.allowScaling; }
	
	/**
	 * Set value enabled. Should be object
	 * drawn or not?
	 * 
	 * @param b Enable?
	 * @since 1.0
	 */
	public void setAllowScaling(boolean b) { this.box.allowScaling = b; }

	/**
	 * Getter for receiving value. Should be object
	 * drawn or not?
	 * 
	 * @return Receiving?
	 * @since 1.0
	 */
	public boolean getReceiving()	{ return this.receiving; }
	
	/**
	 * Set value receiving. Should be object
	 * drawn or not?
	 * 
	 * @param b Receiving?
	 * @since 1.0
	 */
	public void setReceiving(boolean b) { this.receiving = b; }
	
	/**
	 * Min. Separation value is used for desired update delay.
	 * 
	 * @return NtpTime value of Min. Separation.
	 * @since 1.0
	 */
	public NtpTime getMinSeparation()
	{
		return this.subscription.getProperties().getMinSeparation();
	}
	
	/**
	 * Min. Separation value is used for desired update delay,
	 * this is setter for this value.
	 * 
	 * @param newMinSeparation
	 * @since 1.0
	 */
	public void setMinSeparation(NtpTime newMinSeparation)
	{
		SubsProp properties = this.subscription.getProperties();
		properties.setMinSeparation(newMinSeparation);
		this.subscription.setProperties(properties);
	}
	
	/**
	 * Pass parameters to box.
	 * 
	 * @param currentWidth
	 * @param currentHeight
	 * @since 1.0
	 */
	public void setScale(int currentWidth, int currentHeight)
	{
		this.box.setScale(currentWidth, currentHeight);
	}

}
