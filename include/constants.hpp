/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KDOTS_CONSTANTS_HPP
#define KDOTS_CONSTANTS_HPP
#include <QString>
#include <kdemacros.h>
#ifndef KDOTS_EXPORT
# if defined(MAKE_KDOTS_LIB)
   // We are building this library
#  define KDOTS_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define KDOTS_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KDOTS_EXPORT_DEPRECATED
#  define KDOTS_EXPORT_DEPRECATED KDE_DEPRECATED KDOTS_EXPORT
# endif

namespace KDots
{
	enum GameMode
	{
		EXTRA_TURN_MODE,
		DEFAULT_MODE
	};

	enum Owner
	{
		NONE,
		FIRST,
		SECOND
	};

	const int GRAPH_DX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	const int GRAPH_DY[8] = {1, 1, 0, -1, -1, -1, 0, 1};

	const QString PLUGIN_SUFFIX = "kdots_";
}

#endif