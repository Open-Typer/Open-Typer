import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ToolBar {
	property alias control: loader.sourceComponent
	padding: 0

	background: Rectangle { color: themeEngine.panelColor }
	RowLayout {
		width: parent.width
		ColumnLayout {
			Loader { id: loader }
		}
	}
}
