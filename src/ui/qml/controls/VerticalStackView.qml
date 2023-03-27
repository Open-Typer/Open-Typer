/*
 * VerticalStackView.qml
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

StackView {
	property int animationDuration: 250
	id: control
	pushEnter: Transition {
		ParallelAnimation {
			YAnimator {
				from: (control.mirrored ? -1 : 1) * control.height
				to: 0
				duration: animationDuration
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 0
				to: 1
				duration: animationDuration
			}
		}
	}

	pushExit: Transition {
		ParallelAnimation {
			YAnimator {
				from: 0
				to: (control.mirrored ? -1 : 1) * -control.height
				duration: animationDuration
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 1
				to: 0
				duration: animationDuration
			}
		}
	}

	popEnter: Transition {
		ParallelAnimation {
			YAnimator {
				from: (control.mirrored ? -1 : 1) * -control.height
				to: 0
				duration: animationDuration
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 0
				to: 1
				duration: animationDuration
			}
		}
	}

	popExit: Transition {
		ParallelAnimation {
			YAnimator {
				from: 0
				to: (control.mirrored ? -1 : 1) * control.height
				duration: animationDuration
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 1
				to: 0
				duration: animationDuration
			}
		}
	}

	replaceEnter: pushEnter

	replaceExit: pushExit
}
