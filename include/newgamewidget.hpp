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
#ifndef KDOTS_NEWGAMEWIDGET_HPP
#define KDOTS_NEWGAMEWIDGET_HPP
#include <QWidget>
#include "constants.hpp"
#include "gameconfig.hpp"

namespace Ui
{
	class NewGameWidget;
}

namespace KDots
{
	class NewGameWidget : public QWidget
	{
		Q_OBJECT
	public:
		NewGameWidget (QWidget *parent = 0);

		int getHeight () const;
		int getWidth () const;
		GameMode getGameMode () const;
		Owner getFirstMoving () const;

		GameConfig getGameConfig () const;
	private:
		Ui::NewGameWidget *m_ui;
	};
}



#endif