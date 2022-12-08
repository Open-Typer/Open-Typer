import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ToolBar {
	property alias control: loader.sourceComponent
	padding: 0
	function getPanelColor() {
		background.color = themeEngine.panelColor();
		return background.color;
	}
	Connections {
		target: themeEngine
		function onColorChanged() { getPanelColor(); }
		function onStyleChanged() { getPanelColor(); }
		function onThemeChanged() { getPanelColor(); }
	}

	background: Rectangle { color: getPanelColor() }
	RowLayout {
		width: parent.width
		ColumnLayout {
			Loader { id: loader }
		}
	}
}
