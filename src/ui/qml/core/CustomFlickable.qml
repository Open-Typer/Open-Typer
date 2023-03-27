/*
 * CustomFlickable.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.12
import QtQuick.Controls 2.5

Flickable {
	property bool showHorizontalScrollBar: false
	property bool showVerticalScrollBar: false
	id: flickable

	function ensureVisibleX(item) {
		if (moving || dragging)
			return;
		var xpos = item.mapToItem(contentItem, 0, 0).x
		var ext = item.width + xpos
		if (xpos < contentX // begins before
			|| xpos > contentX + width // begins after
			|| ext < contentX // ends before
			|| ext > contentX + width) { // ends after
			// don't exceed bounds
			var destinationX = Math.max(0, Math.min(xpos - width + item.width, contentWidth - width))
			xAnimation.to = destinationX;
			xAnimation.from = contentX;
			xAnimation.start();
		}
	}

	function ensureVisibleY(item) {
		if (moving || dragging)
			return;
		var ypos = item.mapToItem(contentItem, 0, 0).y
		var ext = item.height + ypos
		if (ypos < contentY // begins before
			|| ypos > contentY + height // begins after
			|| ext < contentY // ends before
			|| ext > contentY + height) { // ends after
			// don't exceed bounds
			var destinationY = Math.max(0, Math.min(ypos - height + item.height, contentHeight - height))
			yAnimation.to = destinationY;
			yAnimation.from = contentY;
			yAnimation.start();
		}
	}

	// ensureVisible methods: https://stackoverflow.com/questions/45946637/how-to-make-flickable-ensure-the-visibility-of-an-item-inside-of-it
	function ensureVisible(item) {
		ensureVisibleX(item);
		ensureVisibleY(item);
	}

	function stopAnimations() {
		xAnimation.stop();
		yAnimation.stop();
	}

	NumberAnimation on contentX {
		id: xAnimation
		to: 0
		duration: 200
		easing.type: Easing.InOutQuad;
	}

	NumberAnimation on contentY {
		id: yAnimation
		to: 0
		duration: 200
		easing.type: Easing.InOutQuad;
	}

	ScrollBar.horizontal: ScrollBar {
		width: 10
		position: flickable.visibleArea.xPosition
		policy: showHorizontalScrollBar && interactive ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
	}

	ScrollBar.vertical: ScrollBar {
		width: 10
		position: flickable.visibleArea.yPosition
		policy: showVerticalScrollBar && interactive ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
	}
}
