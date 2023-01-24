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
import OpenTyper 1.0

ColumnLayout {
	property int errorPenalty: Settings.errorPenalty()
	property int lesson
	property int sublesson
	property int exercise

	Label {
		text: qsTr("Error penalty: %1").arg(errorPenalty);
		font.bold: true
		font.pointSize: 12
	}

	ChartView {
		id: chartView
		Layout.fillWidth: true
		Layout.fillHeight: true
		theme: ThemeEngine.theme == ThemeEngine.DarkTheme ? ChartView.ChartThemeDark : ChartView.ChartThemeLight
		backgroundColor: ThemeEngine.paperColor
		antialiasing: true

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
				labelFormat: "%d"
			}
			axisY: ValueAxis {
				labelFormat: "%d"
				max: Math.max(chartView.getMaxY(netHitsSeries), chartView.getMaxY(grossHitsSeries)) + 50
			}

			XYPoint { x: 0; y: 250 }
			XYPoint { x: 1; y: 300 }
			XYPoint { x: 2; y: 335 }
			XYPoint { x: 3; y: 340 }
			XYPoint { x: 4; y: 320 }
		}

		SplineSeries {
			id: grossHitsSeries
			name: qsTr("Gross hits per minute")

			XYPoint { x: 0; y: 275 }
			XYPoint { x: 1; y: 315 }
			XYPoint { x: 2; y: 335 }
			XYPoint { x: 3; y: 350 }
			XYPoint { x: 4; y: 340 }
		}
	}
}
