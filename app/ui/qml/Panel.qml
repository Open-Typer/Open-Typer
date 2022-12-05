import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ToolBar {
	property alias control: loader.sourceComponent
	padding: 0
	background: Rectangle {
		color: "#14212f" // TODO: Use panel color from ThemeEngine
		radius: 5
	}
	RowLayout {
		width: parent.width
		ColumnLayout {
			Loader { id: loader }
		}
	}
}
