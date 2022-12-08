import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15

Rectangle {
	property color materialColor: Material.foreground
	radius: 10
	border.color: Qt.rgba(materialColor.r, materialColor.g, materialColor.b, 0.25)
	border.width: 0.5
	width: 1000
	height: 300
	color: themeEngine.paperColor
	clip: true
	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 20
		spacing: 0
		TextArea {
			Layout.fillWidth: true
			background: null
			topPadding: 0
			text: "Current line"
			font: themeEngine.font
			color: themeEngine.exerciseTextColor
			readOnly: true
			selectByMouse: false
			selectByKeyboard: false
			clip: true
		}
		// MenuSeparator should be enough...
		MenuSeparator {
			Layout.fillWidth: true
		}
		TextArea {
			Layout.fillWidth: true
			background: null
			topPadding: 0
			text: "tes"
			font: themeEngine.font
			color: themeEngine.inputTextColor
			readOnly: false
			selectByMouse: false
			selectByKeyboard: false
			clip: true
			Label {
				anchors.fill: parent
				background: null
				text: " _ t"
				font: themeEngine.font
				color: Qt.rgba(0.84, 0.28, 0.06, 1)
				clip: true
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
				id: remainingText
				background: null
				text: "another line\nanother line\nanother line\nanother line\nanother line\nanother line"
				font: themeEngine.font
				readOnly: true
				selectByMouse: false
				selectByKeyboard: false
				clip: true
				visible: false
			}
			LinearGradient  {
				id: mask
				anchors.fill: remainingText
				source: remainingText
				visible: false
				gradient: Gradient {
					GradientStop { position: 0; color: themeEngine.exerciseTextColor }
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
