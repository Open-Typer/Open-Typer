/*
 * BackgroundBlur.qml
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
import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects 1.0

FastBlur {
	property int depth: 0 // number of opened dialogs
	id: root
	cached: true
	radius: 0
	visible: false

	function show() {
		depth++;
		if(visible)
			return;
		outAnimation.stop();
		inAnimation.start();
		visible = true;
	}

	function hide() {
		depth--;
		if(!visible || depth > 0)
			return;
		inAnimation.stop();
		outAnimation.start();
	}

	PropertyAnimation {
		id: inAnimation
		target: root
		property: "radius"
		to: 40
		duration: 125
	}

	PropertyAnimation {
		id: outAnimation
		target: root
		property: "radius"
		to: 0
		duration: 125
		onFinished: root.visible = false
	}
}
