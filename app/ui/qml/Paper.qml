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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15
import OpenTyper 1.0

Rectangle {
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
	radius: 10
	border.color: Qt.rgba(materialColor.r, materialColor.g, materialColor.b, 0.25)
	border.width: 0.5
	width: textMargins * 2 + Math.max(currentLineText.textWidth, inputText.textWidth, remainingText.textWidth);
	color: ThemeEngine.paperColor
	clip: true
	id: control

	function calculateTextWidth(text, metrics) {
		var lines = text.split("\n");
		var maxValue = 0;
		for(var i = 0; i < lines.length; i++)
			maxValue = Math.max(maxValue, metrics.boundingRect(lines[i]).width);
		return maxValue;
	}

	ColumnLayout {
		anchors.fill: parent
		anchors.margins: control.textMargins
		spacing: 0
		TextArea {
			property int textWidth: calculateTextWidth(text, currentLineTextMetrics);
			id: currentLineText
			Layout.fillWidth: true
			background: null
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
		TextArea {
			property int textWidth: Math.max(calculateTextWidth(text, inputTextMetrics), calculateTextWidth(errorText.text, errorTextMetrics));
			id: inputText
			Layout.fillWidth: true
			background: null
			topPadding: 0
			text: input
			font: ThemeEngine.font
			color: ThemeEngine.inputTextColor
			readOnly: false
			selectByMouse: false
			selectByKeyboard: false
			clip: true
			visible: inputVisible
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
			TextArea {
				property int textWidth: calculateTextWidth(text, remainingTextMetrics);
				id: remainingText
				background: null
				text: remaining
				font: ThemeEngine.font
				readOnly: true
				selectByMouse: false
				selectByKeyboard: false
				clip: true
				visible: false
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
	}
}
