/**
 * 
 *	This file is part of shape4a.
 *
 *  shape4a is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  shape4a is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with shape4a.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
package org.ocera.orte.shape4a;

import org.ocera.orte.*;
import org.ocera.orte.types.*;

import android.util.Log;

/**
 * Object to be send throw ORTE.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class Box extends MessageData
{
	private static final double DESTINATION_WIDTH = 389.0;
	private static final double DESTINATION_HEIGHT = 256.0;
	
	public boolean allowScaling;
	public int color;
	// is more shape than strength,
	// strength is solved throw
	// publisherProperties or
	// throw subscriberProperties...
	public int strength;
	public BoxRect rectangle;
	
	private double scaleWidth;
	private double scaleHeight;
	
	/**
	 * Add topic of new object.
	 * 
	 * @param appDomain
	 * @param newTopic
	 * @since 1.0
	 */
	public Box(DomainApp appDomain, String newTopic) {
		super();
		this.setTopic(newTopic);
		
		if (!appDomain.regNewDataType("Box", this.getMaxDataLength())) {
			Log.e("Box", "Cannot register data type 'Box'.");
		}
		
		this.rectangle = new BoxRect();
		this.allowScaling = true;
		this.scaleWidth = 1;
		this.scaleHeight = 1;
	}

	/**
	 * Read data from buffer.
	 * @since 1.0
	 */
	@Override
	public void read() {
		buffer.rewind();
		if (this.allowScaling) {
			this.rectangle.top_left_x = (short) (buffer.getShort() / this.scaleWidth);
			this.rectangle.top_left_y = (short) (buffer.getShort() / this.scaleHeight);
			this.rectangle.bottom_right_x = (short) (buffer.getShort() / this.scaleWidth);
			this.rectangle.bottom_right_y = (short) (buffer.getShort() / this.scaleHeight);
		} else {
			this.rectangle.top_left_x = buffer.getShort();
			this.rectangle.top_left_y = buffer.getShort();
			this.rectangle.bottom_right_x = buffer.getShort();
			this.rectangle.bottom_right_y = buffer.getShort();
		}
		
		this.strength = (int) buffer.getShort();
	}

	/**
	 * Write data to buffer.
	 * @since 1.0
	 */
	@Override
	public void write() {
		buffer.rewind();
		if (this.allowScaling) {
			buffer.putShort((short) (rectangle.top_left_x * this.scaleWidth));
			buffer.putShort((short) (rectangle.top_left_y * this.scaleHeight));
			buffer.putShort((short) (rectangle.bottom_right_x * this.scaleWidth));
			buffer.putShort((short) (rectangle.bottom_right_y * this.scaleHeight));
		} else {
			buffer.putShort(rectangle.top_left_x);
			buffer.putShort(rectangle.top_left_y);
			buffer.putShort(rectangle.bottom_right_x);
			buffer.putShort(rectangle.bottom_right_y);
		}
		
		buffer.putShort((short) this.strength);
	}

	/**
	 * Get maximum length of data.
	 * @since 1.0
	 */
	@Override
	public int getMaxDataLength() {
		return 5 * ORTEConstant.SHORT_FIELD_SIZE;
	}
	
	/**
	 * Object to be truly send throw ORTE.
	 * 
	 * @since 1.0
	 */
	public class BoxRect
	{
		public short top_left_x;
		public short top_left_y;
		public short bottom_right_x;
		public short bottom_right_y;
		
		public BoxRect()
		{}
	}
	
	/**
	 * When screen rotates, change scale variables to
	 * fit the destination screen.
	 * 
	 * @since 1.0
	 */
	public void setScale(int currentWidth, int currentHeight)
	{
		this.scaleWidth = DESTINATION_WIDTH / currentWidth;
		this.scaleHeight = DESTINATION_HEIGHT / currentHeight;
	}
}
