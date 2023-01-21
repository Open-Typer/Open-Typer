/*
 * KeyboardKey.qml
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
import OpenTyper 1.0

Item {
	property string text
	property string shiftText
	property int type: KeyboardUtils.KeyType_Any
	property color highlightColor: ThemeEngine.theme == ThemeEngine.DarkTheme ? "white" : "black"
	property bool highlighted: false
	property bool pressed: false

	id: root
	width: {
		switch(type)
		{
			case KeyboardUtils.KeyType_Any:
				return 50;
			case KeyboardUtils.KeyType_Tab:
				return 85;
			case KeyboardUtils.KeyType_CapsLock:
				return 100;
			case KeyboardUtils.KeyType_Return:
				return 75;
			case KeyboardUtils.KeyType_LShift:
				return 70;
			case KeyboardUtils.KeyType_RShift:
				return 163;
			case KeyboardUtils.KeyType_Ctrl:
				return 75;
			case KeyboardUtils.KeyType_LAlt:
				return 100;
			case KeyboardUtils.KeyType_RAlt:
				return 100;
			case KeyboardUtils.KeyType_Space:
				return 490;
			case KeyboardUtils.KeyType_Backspace:
				return 125;
		}
		return 50;
	}
	height: Math.max(label.height, 50)
	onHighlightedChanged: {
		if(highlighted)
		{
			dehighlightAnimation.stop();
			highlightAnimation.start();
		}
		else
		{
			highlightAnimation.stop();
			dehighlightAnimation.start();
		}
	}
	onPressedChanged: {
		if(pressed)
		{
			releaseAnimation.stop();
			pressAnimation.start();
		}
		else
		{
			pressAnimation.stop();
			releaseAnimation.start();
		}
	}

	Rectangle {
		readonly property bool isReturn: type == KeyboardUtils.KeyType_Return
		readonly property color keyColor: Qt.rgba(ThemeEngine.currentAccentColor.r, ThemeEngine.currentAccentColor.g, ThemeEngine.currentAccentColor.b, 0.3)
		property real tintAlpha: 0.15
		readonly property color tintColor: Qt.rgba(highlightColor.r, highlightColor.g, highlightColor.b, tintAlpha)
		readonly property int keyWidth: parent.width
		readonly property int keyHeight: isReturn ? 105 : parent.height
		id: keyRect
		color: {
			var finalColor = keyColor;
			if(highlighted)
				finalColor = Qt.tint(finalColor, tintColor);
			if(pressed)
				finalColor = Qt.rgba(finalColor.r, finalColor.g, finalColor.b, 0.4);
			return finalColor;
		}
		radius: 10
		border.width: 2
		border.color: ThemeEngine.theme == ThemeEngine.DarkTheme ? Qt.lighter(color, 1.3) : Qt.darker(color, 1.3)
		width: keyWidth
		height: keyHeight
		y: isReturn ? -55 : 0

		Label {
			id: label
			visible: root.text != root.shiftText
			padding: 5
			anchors.bottom: parent.bottom
			text: root.text
		}

		Label {
			id: shiftLabel
			padding: 5
			anchors.top: parent.top
			text: root.text == root.shiftText ? root.shiftText.toUpperCase() : root.shiftText
		}

		PropertyAnimation {
			id: highlightAnimation
			target: keyRect
			property: "tintAlpha"
			to: 0.15
			duration: 500
			easing.type: Easing.OutCubic
		}

		PropertyAnimation {
			id: dehighlightAnimation
			target: keyRect
			property: "tintAlpha"
			to: 0
			duration: 500
			easing.type: Easing.InCubic
		}

		PropertyAnimation {
			id: pressAnimation
			target: keyRect
			property: "scale"
			to: 0.85
			duration: 125
			easing.type: Easing.OutCubic
		}

		PropertyAnimation {
			id: releaseAnimation
			target: keyRect
			property: "scale"
			to: 1
			duration: 125
			easing.type: Easing.InCubic
		}
	}
}
