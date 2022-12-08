import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import OpenTyper 1.0

Item {
	Material.theme: themeEngine.style === ThemeEngine.DarkStyle ? Material.Dark : Material.Light
	Material.accent: Material.LightBlue // TODO: Use accent color (maybe from ThemeEngine)
	width: mainLayout.width
	height: mainLayout.height
	visible: true
	id: root

	ColumnLayout {
		id: mainLayout
		width: parent.width
		spacing: 0
		Panel {
			Layout.fillWidth: true
			control: RowLayout {
				CustomToolButton {
					objectName: "settingsButton"
					icon.name: "settings"
					toolTipText: qsTr("Options")
				}
				CustomToolButton {
					objectName: "openButton"
					icon.name: "open"
					toolTipText: qsTr("Open")
				}
				CustomToolButton {
					objectName: "printButton"
					icon.name: "print"
					toolTipText: qsTr("Print")
				}
				ToolSeparator {}
				CustomToolButton {
					objectName: "typingTestButton"
					icon.name: "paper"
					text: qsTr("Typing test")
				}
				CustomToolButton {
					objectName: "timedExButton"
					icon.name: "time"
					text: qsTr("Timed exercise")
				}
				CustomToolButton {
					objectName: "errorWordsButton"
					icon.name: "close"
					text: qsTr("Text from error words")
				}
				CustomToolButton {
					objectName: "reverseTextButton"
					icon.name: "rewind"
					text: qsTr("Reverse text")
				}
			}
		}
		Panel {
			Layout.fillWidth: true
			control: RowLayout {
				CustomToolButton {
					objectName: "repeatExButton"
					icon.name: "repeat"
					toolTipText: qsTr("Repeat exercise")
				}
				CustomToolButton {
					objectName: "closeLoadedExButton"
					icon.name: "close"
					toolTipText: qsTr("Close loaded exercise")
				}
				CustomToolButton {
					objectName: "previousExButton"
					icon.name: "left"
					toolTipText: qsTr("Previous exercise")
				}
				CustomToolButton {
					objectName: "nextExButton"
					icon.name: "right"
					toolTipText: qsTr("Next exercise")
				}
				CustomComboBox { objectName: "lessonBox" }
				CustomComboBox { objectName: "sublessonBox" }
				CustomComboBox { objectName: "exerciseBox" }
				ToolSeparator {}
				CustomToolButton {
					objectName: "statsButton"
					icon.name: "stats"
					text: qsTr("Exercise history")
				}
			}
		}
		Paper {
			Layout.fillHeight: true
			Layout.topMargin: 10
			Layout.alignment: Qt.AlignHCenter
		}
	}
}
