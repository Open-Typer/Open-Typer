import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

// Similar to ToolButton, but it's always a rectangle
// Double click events are not supported, use the clicked() signal like with a QPushButton
Button {
	property string toolTipText
	signal clicked()
	font.capitalization: Font.MixedCase
	Material.background: Qt.rgba(0, 0, 0, 0)
	onReleased: clicked()
	HoverToolTip { text: toolTipText }
}
