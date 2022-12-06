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
	color: "#0f1923" // TODO: Use paper color from ThemeEngine
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
			font.family: "FreeMono" // TODO: Use exercise text font family from ThemeEngine
			font.pointSize: 20 // TODO: Use text font size from ThemeEngine
			font.bold: true // TODO: Use text style from ThemeEngine
			color: "#007daf" // TODO: Use exercise text color from ThemeEngine
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
			font.family: "FreeMono" // TODO: Use exercise text font family from ThemeEngine
			font.pointSize: 20 // TODO: Use text font size from ThemeEngine
			font.bold: true // TODO: Use text style from ThemeEngine
			// TODO: Use input text color from ThemeEngine
			readOnly: false
			selectByMouse: false
			selectByKeyboard: false
			clip: true
			Label {
				anchors.fill: parent
				background: null
				text: " _ t"
				font.family: "FreeMono" // TODO: Use exercise text font family from ThemeEngine
				font.pointSize: 20 // TODO: Use text font size from ThemeEngine
				font.bold: true // TODO: Use text style from ThemeEngine
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
				font.family: "FreeMono" // TODO: Use exercise text font family from ThemeEngine
				font.pointSize: 20 // TODO: Use text font size from ThemeEngine
				font.bold: true // TODO: Use text style from ThemeEngine
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
					GradientStop { position: 0; color: Qt.rgba(0, 0.49, 0.69, 1) } // TODO: Use exercise text color from ThemeEngine
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
