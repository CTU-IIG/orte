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

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

import org.ocera.orte.DomainApp;

/**
 * View, where subscriber objects are drawn.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I%, %G%
 * @since 1.0
 */
public class SubscriberView extends View {
	public ArrayList<SubscriberElement> elements = new ArrayList<SubscriberElement>();
	
	/**
	 * Default constructor.
	 * 
	 * @param context
	 * @param attrs
	 */
	public SubscriberView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	/**
	 * When drawing, draw all subscribers shapes.
	 * 
	 * @param canvas
	 */
	@Override
	protected void onDraw(Canvas canvas)
	{
		for (SubscriberElement element : elements) {
			element.lock.lock();
			try {
				if (element.getReceiving() && element.getEnabled()) element.drawMe(canvas);
			} finally {
				element.lock.unlock();
			}
		}
	}
	
	/**
	 * Make objects of all colors ready to subscribe.
	 * 
	 * @param appDomain
	 */
	public void addElements(DomainApp appDomain)
	{
		for (int i = 0; i < 5; i++) {
			this.elements.add(new SubscriberElement(appDomain, i, this));
		}
	}
}
