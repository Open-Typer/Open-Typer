/*
 * packs.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#include "core/packs.h"

/*! Returns human-readable name of the built-in pack. */
QString builtinPacks::packName(QString rawName)
{
	// Human-readable built-in pack names
	if(rawName == "sk_SK-QWERTZ-B1")
		return "SK QWERTZ (ADK) [BETA]";
	else
		return tr("Unknown pack");
}
