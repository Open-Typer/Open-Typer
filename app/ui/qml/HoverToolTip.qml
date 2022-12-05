import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

ToolTip {
	visible: parent.hovered && text != ""
	delay: 500
	background.height: background.implicitHeight // Fix background smaller than tool tip text
}
