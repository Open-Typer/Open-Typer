/*
 * CustomDialog.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 2.3 as NewControls
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects 1.0
import OpenTyper 1.0

Item {
	property Item blurSource: QmlUtils.blurSource
	property string windowTitle: ""
	property int standardButtons: Dialog.NoButton
	readonly property var dialogColor: Material.theme === Material.Light ? "white" : "black"
	readonly property alias dialog: control
	property alias contentComponent: contentsLoader.sourceComponent
	readonly property alias contentItem: contentsLoader.item
	property bool draggable: true
	property bool fillWindow: false
	signal accepted()
	// TODO: Add signals introduced in Qt 5.10
	signal rejected()
	signal aboutToShow()
	signal aboutToHide()
	id: root
	anchors.fill: parent

	function open() {
		control.open();
	}

	Rectangle {
		id: dialogMask
		width: control.width + 4
		height: control.height + 4
		color: dialogColor
		radius: control.radius
		visible: shadow.visible
		border.width: 1
		border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
		Component.onCompleted: resetPos();
		onXChanged: checkPos();
		onYChanged: checkPos();

		function resetPos() {
			anchors.centerIn = parent;
			parent.onWidthChanged.connect(function() { checkPos(); });
			parent.onHeightChanged.connect(function() { checkPos(); });
		}

		function checkPos() {
			if(anchors.centerIn == parent)
				return;
			// x
			var finalX = x;
			if(finalX < 0)
				finalX = 0;
			else if(finalX + width > parent.width)
				finalX = parent.width - width;
			if(finalX != x)
				x = finalX;
			// y
			var finalY = y;
			if(finalY < 0)
				finalY = 0;
			if(finalY + height > parent.height)
				finalY = parent.height - height;
			if(finalY != y)
				y = finalY;
		}
	}

	FastBlur {
		id: blur
		anchors.fill: root
		source: blurSource
		visible: false
		radius: 0
		PropertyAnimation {
			id: blurInAnimation
			target: blur
			property: "radius"
			to: 40
			duration: 125
		}
		PropertyAnimation {
			id: blurOutAnimation
			target: blur
			property: "radius"
			to: 0
			duration: 125
		}
	}

	DropShadow {
		id: shadow
		anchors.fill: dialogMask
		horizontalOffset: 20
		verticalOffset: 20
		radius: 16
		samples: 16
		color: "#80000000"
		source: dialogMask
		opacity: 0
		visible: false
		PropertyAnimation {
			id: shadowInAnimation
			target: shadow
			property: "opacity"
			to: 1.0
			duration: 125
		}
		PropertyAnimation {
			id: shadowOutAnimation
			target: shadow
			property: "opacity"
			to: 0.0
			duration: 125
		}
	}

	Dialog {
		property int radius: 10
		id: control
		x: dialogMask.x + 2
		y: dialogMask.y + 2
		width: fillWindow ? parent.width - 100 : headerLayout.implicitWidth
		height: fillWindow ? parent.height - 100 : (standardButtons !== Dialog.NoButton ? headerLayout.implicitHeight + dialogButtonBox.height : headerLayout.implicitHeight)
		standardButtons: root.standardButtons
		modal: true
		onAboutToShow: {
			blur.visible = true;
			blurInAnimation.running = true;
			shadowInAnimation.running = true;
			shadow.visible = true;
			root.aboutToShow();
		}
		onAboutToHide: {
			blurOutAnimation.running = true;
			shadowOutAnimation.running = true;
			shadow.visible = false
			root.aboutToHide();
		}
		onClosed: {
			blur.visible = false;
			dialogMask.resetPos();
			QmlUtils.reloadMenuBar();
		}
		header: Rectangle {
			color: dialogColor
			width: control.width
			height: standardButtons !== Dialog.NoButton ? control.height - control.footer.height : control.height
			radius: control.radius
			ColumnLayout {
				id: headerLayout
				anchors.fill: parent
				spacing: 0
				Label {
					id: windowTitleLabel
					Layout.fillWidth: true
					padding: 0
					text: root.windowTitle
					horizontalAlignment: Qt.AlignHCenter
					visible: text != ""
					Drag.active: draggable
					MouseArea {
						enabled: draggable
						anchors.fill: parent
						drag.target: dialogMask
						onPressed: dialogMask.anchors.centerIn = undefined;
					}
				}
				MenuSeparator {
					Layout.fillWidth: true
					visible: root.windowTitle != ""
				}
				Pane {
					padding: 10
					background: null
					clip: true
					Layout.fillWidth: true
					Layout.fillHeight: true
					Loader {
						id: contentsLoader
						anchors.fill: parent
					}
				}
				MenuSeparator {
					Layout.fillWidth: true
					visible: standardButtons !== Dialog.NoButton
				}
			}
		}
		footer: DialogButtonBox {
			id: dialogButtonBox
			font.capitalization: Font.MixedCase
			background: Rectangle {
				color: dialogColor
				radius: control.radius
			}
		}
		background: Rectangle {
			color: dialogColor
			width: control.width
			height: control.height
			radius: control.radius
		}
		Component {
			id: overlayModal
			Rectangle {
				color: Qt.rgba(0, 0, 0, 0.25)
			}
		}
		Component.onCompleted: {
			if((QmlUtils.qtVersionMajor() === 5) && (QmlUtils.qtVersionMinor() < 10))
				overlay.modal = overlayModal;
			else
				NewControls.Overlay.modal = overlayModal;
			control.dim = false;
			control.dim = true;
		}
		onAccepted: root.accepted()
		onRejected: root.rejected()
	}
}
