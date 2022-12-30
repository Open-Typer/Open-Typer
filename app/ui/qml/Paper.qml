/*
 * Paper.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects 1.0
import OpenTyper 1.0

Item {
	property alias paperRect: paperRect
	property color materialColor: Material.foreground
	property int textMargins: 20
	property string currentLine: ""
	property bool currentLineVisible: true
	property string input: ""
	property bool inputVisible: true
	property string mistake: ""
	property bool mistakeVisible: true
	property string remaining: ""
	property bool remainingVisible: true
	property alias summary: summary
	signal keyPressed(var event)
	signal keyReleased(var event)
	id: control

	function calculateTextWidth(text, metrics) {
		var lines = text.split("\n");
		var maxValue = 0;
		for(var i = 0; i < lines.length; i++)
			maxValue = Math.max(maxValue, metrics.boundingRect(lines[i]).width);
		return maxValue;
	}

	KeyboardHandler {
		id: keyboardHandler
		onKeyPressed: control.keyPressed(event);
		onKeyReleased: control.keyReleased(event);
	}

	Rectangle {
		id: paperRect
		radius: 10
		border.color: Qt.rgba(materialColor.r, materialColor.g, materialColor.b, 0.25)
		border.width: 0.5
		anchors.centerIn: parent
		width: textMargins * 3 + Math.max(currentLineText.textWidth, inputText.textWidth, remainingText.textWidth);
		height: parent.height
		color: ThemeEngine.paperColor
		clip: true

		ColumnLayout {
			anchors.fill: parent
			anchors.margins: control.textMargins
			spacing: 0
			TextEdit {
				property int textWidth: calculateTextWidth(text, currentLineTextMetrics);
				id: currentLineText
				Layout.fillWidth: true
				topPadding: 0
				text: currentLine
				font: ThemeEngine.font
				color: ThemeEngine.exerciseTextColor
				readOnly: true
				selectByMouse: false
				selectByKeyboard: false
				clip: true
				visible: currentLineVisible
				FontMetrics {
					id: currentLineTextMetrics
					font: currentLineText.font
				}
			}
			// MenuSeparator should be enough...
			MenuSeparator {
				Layout.fillWidth: true
				visible: currentLineVisible
			}
			TextEdit {
				property int textWidth: Math.max(calculateTextWidth(text, inputTextMetrics), calculateTextWidth(errorText.text, errorTextMetrics));
				property bool lockFocus: false
				property bool cursorBlinkState: true
				id: inputText
				Layout.fillWidth: true
				Layout.alignment: Qt.AlignTop
				topPadding: 0
				text: input
				font: ThemeEngine.font
				color: ThemeEngine.inputTextColor
				readOnly: false
				selectByMouse: false
				selectByKeyboard: false
				clip: true
				visible: inputVisible
				onFocusChanged: {
					if(focus && !lockFocus)
					{
						forceActiveFocus();
						cursorVisible = true;
						keyboardHandler.focus = true;
					}
				}
				onCursorVisibleChanged: {
					if(!cursorVisible)
						cursorVisible = true;
				}
				onTextChanged: {
					lockFocus = true;
					forceActiveFocus();
					cursorPosition = text.length;
					lockFocus = false;
					keyboardHandler.focus = true;
					cursorTimer.restart();
					cursorBlinkState = true;
				}
				cursorDelegate: Rectangle {
					visible: parent.cursorVisible && parent.cursorBlinkState
					color: Material.foreground
					width: parent.cursorRectangle.width
				}

				Timer {
					id: cursorTimer
					interval: 500
					repeat: true
					running: true
					onTriggered: parent.cursorBlinkState = !parent.cursorBlinkState;
				}

				MouseArea {
					anchors.fill: parent
					cursorShape: Qt.IBeamCursor
					onClicked: parent.forceActiveFocus();
				}
				Label {
					id: errorText
					anchors.fill: parent
					background: null
					text: mistake
					font: ThemeEngine.font
					color: Qt.rgba(0.84, 0.28, 0.06, 1)
					clip: true
					visible: mistakeVisible
					FontMetrics {
						id: errorTextMetrics
						font: errorText.font
					}
				}
				FontMetrics {
					id: inputTextMetrics
					font: inputText.font
				}
			}
			Frame {
				// The Label needs to be inside an Item for opacity gradient to work
				Layout.fillWidth: true
				Layout.fillHeight: true
				spacing: 0
				padding: 0
				background: null
				visible: remainingVisible
				TextEdit {
					property int textWidth: calculateTextWidth(text, remainingTextMetrics);
					id: remainingText
					text: remaining
					font: ThemeEngine.font
					readOnly: true
					selectByMouse: false
					selectByKeyboard: false
					clip: true
					visible: false
					height: parent.height * 0.75
					FontMetrics {
						id: remainingTextMetrics
						font: remainingText.font
					}
				}
				LinearGradient  {
					id: mask
					anchors.fill: remainingText
					source: remainingText
					visible: false
					gradient: Gradient {
						GradientStop { position: 0; color: ThemeEngine.exerciseTextColor }
						GradientStop { position: 1; color: Qt.rgba(0, 0, 0, 0) }
					}
				}
				OpacityMask {
					anchors.fill: remainingText
					source: mask
					maskSource: remainingText
				}
			}
			ExerciseSummary {
				id: summary
				Layout.fillWidth: true
				Layout.fillHeight: true
			}
		}
	}
}
