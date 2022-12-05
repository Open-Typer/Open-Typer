import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

// Similar to ToolButton, but it's always a rectangle
Button {
	property string toolTipText
	font.capitalization: Font.MixedCase
	Material.background: Qt.rgba(0, 0, 0, 0)
	HoverToolTip { text: toolTipText }
}
