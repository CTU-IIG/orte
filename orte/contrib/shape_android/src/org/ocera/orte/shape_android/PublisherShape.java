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

import org.ocera.orte.DomainApp;
import org.ocera.orte.Publication;
import org.ocera.orte.types.NtpTime;
import org.ocera.orte.types.PublProp;

import android.graphics.Color;
import android.graphics.Path;
import android.graphics.Path.FillType;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.graphics.drawable.shapes.PathShape;
import android.graphics.drawable.shapes.RectShape;
import android.graphics.drawable.shapes.Shape;

/**
 * Some additional parameters for {@link ShapeDrawable}
 * to make it Publisher-friendly.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class PublisherShape extends ShapeDrawable
{
	// Prefer at least 48x48 shape size (Full asset), for more
	//  info, please refer to http://developer.android.com/
	
	private boolean manual;
	private int incX, incY;
	
	private PublProp publisherProperties;
	private NtpTime persistence;
	
	public Publication publication;
	public BoxType box;
	
	/**
	 * Set new {@link Shape}, strength and color.
	 * 
	 * @param s strength, shape
	 * @param c color
	 * @param appDomain
	 * @since 1.0
	 */
	public PublisherShape(int s, int c, DomainApp appDomain)
	{
		super((s == 0)
				?new RectShape()
				:((s == 1)
					?new OvalShape()
					:((s == 2)
						?new PathShape(new  Path(), PublisherActivity.SHAPE_WIDTH, PublisherActivity.SHAPE_HEIGHT)
						:new RectShape()
					)
				)
			);
		
		if (s == 2) { // fix triangle
			this.setShape(new PathShape(PublisherShape.returnTrianglePath(), 2*PublisherActivity.SHAPE_WIDTH, 2*PublisherActivity.SHAPE_HEIGHT));
		}
		
		this.manual = false;
		this.persistence = new NtpTime(5);

		this.box = new BoxType(appDomain, PublisherShape.getColorName(c));
		this.publisherProperties = new PublProp(this.box.getTopic(),
				"BoxType",
				this.persistence,
				s+1);
		this.publication = appDomain.createPublication(this.publisherProperties, this.box);
		
		this.box.shape = s;
		this.box.color = c;

		this.getPaint().setColor(PublisherShape.getColorConstant(c));
		this.setPadding(0, 0, 0, 0);
		this.setBounds(0, 0, PublisherActivity.SHAPE_WIDTH, PublisherActivity.SHAPE_HEIGHT);
		
		//Log.d("PublisherShape", "s: "+s+", c: "+c+", left: "+this.getBounds().left+", top: "+this.getBounds().top+", right: "+this.getBounds().right+", bottom: "+this.getBounds().bottom);
	}
	
	/**
	 * Update parameters of object to send.
	 * 
	 * @return Object to send.
	 * @since 1.0
	 */
	public BoxType toSend()
	{
		this.box.rectangle.top_left_x = (short) this.getBounds().left;
		this.box.rectangle.top_left_y = (short) this.getBounds().top;
		this.box.rectangle.bottom_right_x = (short) this.getBounds().right;
		this.box.rectangle.bottom_right_y = (short) this.getBounds().bottom;
		
		return this.box;
	}
	
	/**
	 * Revert proprietary color to android color constant.
	 * 
	 * @param c Proprietary value of color.
	 * @return Android color constant.
	 * @since 1.0
	 */
	public static int getColorConstant(int c)
	{
		switch (c) {
		case 0:
			return Color.BLUE;
		case 1:
			return Color.GREEN;
		case 2:
			return Color.RED;
		case 3:
			return Color.BLACK;
		case 4:
			return Color.YELLOW;
		default:
			return 0;
		}
	}
	
	/**
	 * Revert proprietary color to android color constant.
	 * 
	 * @return Android color constant.
	 * @since 1.0
	 */
	public int getColorConstant()
	{
		return PublisherShape.getColorConstant(this.box.color);
	}
	
	/**
	 * Revert proprietary color to word.
	 * 
	 * @param c Proprietary value of color.
	 * @return Name of color.
	 * @since 1.0
	 */
	public static String getColorName(int c)
	{
		// should be done better way
		// texts shouldn't be static
		// but dynamically mapped to
		// strings.xml
		
		switch (c) {
		case 0:
			return "Blue";
		case 1:
			return "Green";
		case 2:
			return "Red";
		case 3:
			return "Black";
		case 4:
			return "Yellow";
		default:
			return "NONE";
		}
	}
	
	/**
	 * Revert proprietary color to word.
	 * 
	 * @return Name of color.
	 * @since 1.0
	 */
	public String getColorName()
	{
		return PublisherShape.getColorName(this.box.color);
	}
	
	/**
	 * Revert strength to shape.
	 * 
	 * @return Name of shape.
	 * @since 1.0
	 */
	public String getShapeName()
	{
		switch (this.box.shape) {
		case 0:
			return "Square";
		case 1:
			return "Circle";
		case 2:
			return "Triangle";
		default:
			return "NONE";
		}
	}
	
	/* getters, setters */
	
	/**
	 * Set value of incX - how much move object
	 * in x-axis direction.
	 * 
	 * @param x
	 * @since 1.0
	 */
	public void setIncX(int x) { this.incX = x; }
	/**
	 * Get value of incX - how much move object
	 * in x-axis direction.
	 * 
	 * @return Distance to move in x-axis. 
	 * @since 1.0
	 */
	public int getIncX() { return this.incX; }
	
	/**
	 * Set value of incY - how much move object
	 * in y-axis direction.
	 * 
	 * @param y
	 * @since 1.0
	 */
	public void setIncY(int y) { this.incY = y; }
	/**
	 * Get value of incY - how much move object
	 * in y-axis direction.
	 * 
	 * @return Distance to move in y-axis. 
	 * @since 1.0
	 */
	public int getIncY() { return this.incY; }
	
	/**
	 * Set strength of publishing.
	 * 
	 * @param s
	 * @since 1.0
	 */
	public void setStrength(int s) {
		PublProp properties = this.publication.getProperties();
		properties.setStrength(s);
		this.publication.setProperties(properties);
	}
	/**
	 * Get strength of publishing.
	 * 
	 * @return Strength of publishing.
	 * @since 1.0
	 */
	public int getStrength() { return this.publication.getProperties().getStrength(); }
	
	/**
	 * Set if object is moved automatically or by
	 * user.
	 * 	TRUE - by user
	 *  FALSE - automatically
	 *  
	 * @param b
	 * @since 1.0
	 */
	public void setManual(boolean b) { this.manual = b; }
	/**
	 * Get if object is moved automatically or by
	 * user.
	 * 
	 * @return How is object moved.
	 * @since 1.0
	 */
	public boolean getManual() { return this.manual; }

	/**
	 * Make triangle path for new instance.
	 * 
	 * @return Triangle path.
	 * @since 1.0
	 */
	public static Path returnTrianglePath()
	{
		Path path = new Path();
		path.setFillType(FillType.EVEN_ODD);
		path.moveTo(PublisherActivity.SHAPE_WIDTH, 0); // triangle top center
		path.lineTo(2*PublisherActivity.SHAPE_WIDTH, 2*PublisherActivity.SHAPE_HEIGHT); // triangle right bottom corner
		path.lineTo(0, 2*PublisherActivity.SHAPE_HEIGHT); // triangle left bottom corner
		path.lineTo(PublisherActivity.SHAPE_WIDTH, 0); // back to triangle top center
		path.close();
		
		return path;
	}
	
	/**
	 * When publisher is removed, destroy publication.
	 * 
	 * @since 1.0
	 */
	public void killMe()
	{
		if (this.publication != null) {
			this.publication.destroy();
			this.publication = null;
		}
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
		this.setBounds(this.box.rectangle.top_left_x, this.box.rectangle.top_left_y, this.box.rectangle.top_left_x+PublisherActivity.SHAPE_WIDTH, this.box.rectangle.top_left_y+PublisherActivity.SHAPE_HEIGHT);
	}
}
