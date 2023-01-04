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

import QtQuick 2.9
import QtQuick.Controls 2.2

StackView {
	id: control
	pushEnter: Transition {
		ParallelAnimation {
			YAnimator {
				from: (control.mirrored ? -1 : 1) * control.height
				to: 0
				duration: 250
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 0
				to: 1
				duration: 250
			}
		}
	}

	pushExit: Transition {
		ParallelAnimation {
			YAnimator {
				from: 0
				to: (control.mirrored ? -1 : 1) * -control.height
				duration: 250
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 1
				to: 0
				duration: 250
			}
		}
	}

	popEnter: Transition {
		ParallelAnimation {
			YAnimator {
				from: (control.mirrored ? -1 : 1) * -control.height
				to: 0
				duration: 250
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 0
				to: 1
				duration: 250
			}
		}
	}

	popExit: Transition {
		ParallelAnimation {
			YAnimator {
				from: 0
				to: (control.mirrored ? -1 : 1) * control.height
				duration: 250
				easing.type: Easing.OutCubic
			}
			PropertyAnimation {
				property: "opacity"
				from: 1
				to: 0
				duration: 250
			}
		}
	}

	replaceEnter: pushEnter

	replaceExit: pushExit
}
