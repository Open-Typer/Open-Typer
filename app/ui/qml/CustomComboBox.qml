import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

// An auto-resizing ComboBox (based on its items)
// Fixes: material style popup background color
// The background is semi-transparent to match with CustomToolButton's design
ComboBox {
	id: control
	FontMetrics {
		id: fontMetric
		font: control.contentItem.font
	}
	Binding on implicitWidth {
		value: computeWidth(model)
	}
	Material.background: Qt.rgba(0, 0.3921, 1, 0.15) // TODO: Use accent color
	popup.background: Rectangle {
		color: backgroundColor()
	}
	popup.implicitWidth: computeWidth(model)
	function computeWidth(mdl) {
		if (mdl === null || mdl === undefined)
			return 100; // width of an empty combo box
		var longest = "";
		for (var i = 0; i < mdl.length; i++) {
			var txt = mdl[i];
			if (txt.length > longest.length)
				longest = txt;
		}
		var pwidth = fontMetric.boundingRect(longest).width;
		pwidth += control.contentItem.rightPadding + control.contentItem.leftPadding;
		pwidth += control.indicator.width
		return pwidth;
	}
	function backgroundColor() {
		if(control.Material.theme === Material.Light)
			return Qt.rgba(1, 1, 1, 0.85);
		else
			return Qt.rgba(0, 0, 0, 0.85);
	}
}
