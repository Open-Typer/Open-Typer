/*
 * MessageBox.qml
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
	property Item blurSource
	property string windowTitle: ""
	property string title: ""
	property int standardButtons: Dialog.Ok
	readonly property var dialogColor: Material.theme === Material.Light ? "white" : "black"
	id: root

	function open() {
		control.open();
	}

	Rectangle {
		id: dialogMask
		anchors.centerIn: parent
        width: control.width + 4
        height: control.height + 4
		color: dialogColor
		radius: control.radius
		visible: shadow.visible
		border.width: 1
		border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
	}

	FastBlur {
		id: blur
		anchors.fill: root
		source: blurSource
		visible: true
		radius: 0
		PropertyAnimation {
			id: blurInAnimation
			target: blur
			property: "radius"
			to: 40
			duration: 250
		}
		PropertyAnimation {
			id: blurOutAnimation
			target: blur
			property: "radius"
			to: 0
			duration: 250
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
			duration: 250
		}
		PropertyAnimation {
			id: shadowOutAnimation
			target: shadow
			property: "opacity"
			to: 0.0
			duration: 250
		}
	}

	Dialog {
		property int radius: 10
		id: control
        x: dialogMask.x + 2
        y: dialogMask.y + 2
		width: headerLayout.implicitWidth
		height: headerLayout.implicitHeight + dialogButtonBox.height
		title: root.title
		standardButtons: root.standardButtons
		modal: true
		onAboutToShow: {
			blurInAnimation.running = true;
			shadowInAnimation.running = true;
			shadow.visible = true;
		}
		onAboutToHide: {
			blurOutAnimation.running = true;
			shadowOutAnimation.running = true;
			shadow.visible = false
		}
        onClosed: QmlUtils.reloadMenuBar();
		header: Rectangle {
			color: dialogColor
			width: headerLayout.implicitWidth
			height: headerLayout.implicitHeight
			radius: control.radius
			ColumnLayout {
				id: headerLayout
				spacing: 0
				Label {
					id: windowTitleLabel
					Layout.fillWidth: true
					padding: 0
					text: root.windowTitle
					horizontalAlignment: Qt.AlignHCenter
					visible: text != ""
				}
				MenuSeparator {
					Layout.fillWidth: true
					visible: root.windowTitle != ""
				}
				Label {
					id: titleLabel
					padding: 10
					text: control.title
					font.bold: true
					font.pointSize: 14
				}
				MenuSeparator { Layout.fillWidth: true }
			}
		}
		footer: DialogButtonBox {
			id: dialogButtonBox
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
		Component.onCompleted: {
			if((QmlUtils.qtVersionMajor() === 5) && (QmlUtils.qtVersionMinor() < 10))
				overlay.modal = null;
			else
				NewControls.Overlay.modal = null;
		}
	}
}
