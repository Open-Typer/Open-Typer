 /*
 * ExportDialog.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.12
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.Export 1.0
import OpenTyper.Validator 1.0

CustomDialog {
	readonly property string studentName: contents.studentName
	readonly property string className: contents.className
	readonly property string testNumber: contents.testNumber
	readonly property string grade: contents.grade
	property ExerciseValidator validator
	id: root
	title: qsTr("Export text")
	standardButtons: Dialog.Close
	fixedSize: false
	maximized: true
	onAboutToShow: repaintTimer.start()

	ExportProvider {
		id: provider
		validator: root.validator
		table: ExportTable
	}

	Timer {
		id: repaintTimer
		interval: 16
		repeat: false
		onTriggered: ExportTable.updateModel()
	}

	contentItem: ColumnLayout {
		readonly property string studentName: studentNameField.text
		readonly property string className: classNameField.text
		readonly property string testNumber: testNumberField.text
		readonly property string grade: gradeField.text
		RowLayout {
			AccentButton {
				text: qsTr("Print")
				icon.name: "print"
				visible: !QmlUtils.osWasm()
				onClicked: provider.print()
			}
		}

		GridLayout {
			columns: 2

			Label {
				text: qsTr("Name:")
				Layout.alignment: Qt.AlignRight
			}

			TextField {
				id: studentNameField
				Layout.fillWidth: true
				maximumLength: 64
			}

			Label {
				text: qsTr("Class:")
				Layout.alignment: Qt.AlignRight
			}

			TextField {
				id: classNameField
				Layout.fillWidth: true
				maximumLength: 16
			}

			Label {
				text: qsTr("Typing test number:")
				Layout.alignment: Qt.AlignRight
			}

			TextField {
				id: testNumberField
				Layout.fillWidth: true
				maximumLength: 32
			}

			Label {
				text: qsTr("Grade:")
				Layout.alignment: Qt.AlignRight
			}

			TextField {
				id: gradeField
				Layout.fillWidth: true
				maximumLength: 8
			}
		}

		Frame {
			Layout.fillWidth: true
			Layout.fillHeight: true
			clip: true
			background: Rectangle {
				radius: 10
				border.color: ThemeEngine.theme == ThemeEngine.DarkTheme ? Qt.rgba(1, 1, 1, 0.25) : Qt.rgba(0, 0, 0, 0.25)
				color: "transparent"
			}

			Flickable {
				anchors.fill: parent
				contentWidth: exportLayout.width
				contentHeight: exportLayout.height

				ColumnLayout {
					id: exportLayout
					spacing: 10

					Label {
						text: provider.exportText
						font.family: ThemeEngine.fontFamily
						font.bold: true
						font.pointSize: 14
					}

					Widget {
						id: tableWidget
						width: ExportTable.contentWidth
						height: ExportTable.contentHeight
						widget: ExportTable

						Binding {
							target: ExportTable
							property: "studentName"
							value: root.studentName
						}

						Binding {
							target: ExportTable
							property: "className"
							value: root.className
						}

						Binding {
							target: ExportTable
							property: "testNumber"
							value: root.testNumber
						}

						Binding {
							target: ExportTable
							property: "grade"
							value: root.grade
						}

						Binding {
							target: ExportTable
							property: "validator"
							value: root.validator
						}

						Connections {
							target: ThemeEngine
							onThemeChanged: {
								ExportTable.updateStyle();
								tableWidget.update();
							}
						}

						Connections {
							target: ExportTable
							onModelChanged: {
								tableWidget.update();
							}
						}
					}
				}
			}
		}
	}
}
