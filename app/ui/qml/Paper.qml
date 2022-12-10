import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15
import OpenTyper 1.0

Rectangle {
	property color materialColor: Material.foreground
	property int textMargins: 20
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
			text: "Current line"
			font: ThemeEngine.font
			color: ThemeEngine.exerciseTextColor
			readOnly: true
			selectByMouse: false
			selectByKeyboard: false
			clip: true
			FontMetrics {
				id: currentLineTextMetrics
				font: currentLineText.font
			}
		}
		// MenuSeparator should be enough...
		MenuSeparator {
			Layout.fillWidth: true
		}
		TextArea {
			property int textWidth: Math.max(calculateTextWidth(text, inputTextMetrics), calculateTextWidth(errorText.text, errorTextMetrics));
			id: inputText
			Layout.fillWidth: true
			background: null
			topPadding: 0
			text: "tes"
			font: ThemeEngine.font
			color: ThemeEngine.inputTextColor
			readOnly: false
			selectByMouse: false
			selectByKeyboard: false
			clip: true
			Label {
				id: errorText
				anchors.fill: parent
				background: null
				text: " _ t"
				font: ThemeEngine.font
				color: Qt.rgba(0.84, 0.28, 0.06, 1)
				clip: true
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
			TextArea {
				property int textWidth: calculateTextWidth(text, remainingTextMetrics);
				id: remainingText
				background: null
				text: "another line\nanother line\nanother line\nanother line\nanother line\nanother line"
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
