/*
 * ExerciseHistory
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
import QtQuick.Layouts 1.12
import QtCharts 2.3
import OpenTyper.Ui 1.0
import OpenTyper.History 1.0

ColumnLayout {
	id: root
	property int errorPenalty: Settings.errorPenalty()
	property string lessonPack
	property int lesson
	property int sublesson
	property int exercise
	onErrorPenaltyChanged: loadData()

	function loadData() {
		netHitsSeries.clear();
		grossHitsSeries.clear();
		var count = parser.count;
		for(var i = 0; i < count; i++)
		{
			var entry = parser.at(i);
			var grossHits = entry.grossHitsPerMinute;
			var mins = entry.timeSecs / 60;
			var netHits;
			if(mins == 0)
				netHits = 0;
			else
				netHits = (grossHits * mins - entry.mistakes * errorPenalty) / mins;
			netHitsSeries.append(i + 1, netHits);
			grossHitsSeries.append(i + 1, grossHits);
		}
		netHitsSeries.axisX.min = 1;
		netHitsSeries.axisX.max = count;
		netHitsSeries.axisX.tickCount = Math.min(count, 15);
		var maxY = Math.max(chartView.getMaxY(netHitsSeries), chartView.getMaxY(grossHitsSeries));
		netHitsSeries.axisY.max = maxY + 50 * (maxY / 250);
	}

	HistoryParser {
		id: parser
		lessonPack: root.lessonPack
		lesson: root.lesson
		sublesson: root.sublesson
		exercise: root.exercise
		onLessonPackChanged: loadData()
		onLessonChanged: loadData()
		onSublessonChanged: loadData()
		onExerciseChanged: loadData()
	}

	Label {
		text: qsTr("Error penalty: %1").arg(errorPenalty);
		font.bold: true
		font.pointSize: 12
	}

	Timer {
		/*
		 * Fixes incorrect background color after
		 * switching to dark theme from menu bar.
		 */
		id: bgColorTimer
		interval: 16
		repeat: false
		onTriggered: ThemeEngine.paperColorChanged()
	}

	ChartView {
		id: chartView
		Layout.fillWidth: true
		Layout.fillHeight: true
		theme: ThemeEngine.theme == ThemeEngine.DarkTheme ? ChartView.ChartThemeDark : ChartView.ChartThemeLight
		backgroundColor: ThemeEngine.paperColor
		antialiasing: true
		visible: parser.count > 0
		onBackgroundColorChanged: bgColorTimer.start()

		function getMaxY(series) {
			var out = 0;
			for(var i = 0; i < series.count; i++)
				out = Math.max(series.at(i).y, out);
			return out;
		}

		SplineSeries {
			id: netHitsSeries
			name: qsTr("Net hits per minute")
			axisX: ValueAxis {
				titleText: qsTr("Attempt")
				labelFormat: "%d"
			}
			axisY: ValueAxis {
				labelFormat: "%d"
			}
		}

		SplineSeries {
			id: grossHitsSeries
			name: qsTr("Gross hits per minute")
		}
	}

	Label {
		//: When there's nothing to show in the exercise history chart
		text: qsTr("No data yet...")
		Layout.fillWidth: true
		Layout.fillHeight: true
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
		visible: parser.count == 0
		font.pointSize: 20
	}
}
