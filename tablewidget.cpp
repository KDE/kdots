/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "tablewidget.hpp"
#include <cmath>
#include <vector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>
#include "dottable.hpp"
#include "stepqueue.hpp"
#include "brushcombodelegate.hpp"
#include "kdots.h"
#include "graph.hpp"

namespace KDots
{
	TableWidget::TableWidget (const GameConfig& config,
			std::shared_ptr<IRival> rival, QWidget *parent)
		: QWidget (parent)
		, m_table (new DotTable (config , this))
		, m_height (config.m_height + 1)
		, m_width (config.m_width + 1)
		, m_rival (rival)
	{
		setMinimumSize (400, 400);

		rival->setDotTable (m_table);

		connect (m_table,
		         SIGNAL (nextPlayer (const Point&)),
		         rival.get (),
		         SLOT (nextStep (const Point&)));
		
		connect (m_table,
				SIGNAL (nextPlayer(const Point&)),
				this,
				SLOT (update ()));
		connect (m_table,
				SIGNAL (nextPlayer (const Point&)),
				this,
				SLOT (onStatusMessage ()));
	}

	namespace
	{
		float cell_size (const QRect& rectange, int height, int width)
		{
			const float cellHeight = rectange.height () / height;
			const float cellWidth = rectange.width () / width;

			return cellHeight > cellWidth ? cellWidth : cellHeight;
		}
	}
	
	void TableWidget::onStatusMessage ()
	{
		emit updateStatusBar (QString ("First:\t")
				+ QString::number (m_table->stepQueue ()->getMarks (FIRST))
				+ "\tSecond:\t"
				+ QString::number (m_table->stepQueue ()->getMarks (SECOND)));
	}

	void TableWidget::mousePressEvent (QMouseEvent *event)
	{
		if (!m_rival->isAllow ())
			return;
		
		const QRect& rectange = rect ();

		const float cellSize = cell_size (rectange, m_height, m_width);

		float dx = (rectange.width () - cellSize * m_width) / 2;

		float dy = (rectange.height () - cellSize * m_height) / 2;

		int x = (event->x () - dx) / cellSize;

		int y = (event->y () - dy) / cellSize;

		const float firstPart = cellSize / 3;

		const float lastPart = cellSize * 2 / 3;

		dx = event->x () - dx - x * cellSize;

		if (dx > firstPart && dx < lastPart)
			return;
		else if (dx < firstPart)
			--x;

		dy = event->y () - dy - y * cellSize;

		if (dy > firstPart && dy < lastPart)
			return;
		else if (dy < firstPart)
			--y;

		if (x >= m_width - 1 || x < 0 || y < 0 || y >= m_height - 1)
			return;

		m_table->pushPoint (Point (x, y));
	}
	
	void TableWidget::undo ()
	{
		setUpdatesEnabled (false);
		m_table->undo ();
		setUpdatesEnabled (true);
	}

	void TableWidget::paintEvent (QPaintEvent *event)
	{
		const QRect& rectange = event->rect ();
		const float cellSize = cell_size (rectange, m_height, m_width);

		const float tableWidth = cellSize * m_width;
		const float tableHeight = cellSize * m_height;

		QPixmap pixmap (QSize (tableWidth, tableHeight));
		pixmap.fill (Qt::white);

		QPainter pixPainter (&pixmap);
		pixPainter.setRenderHint (QPainter::Antialiasing);
		pixPainter.setPen (QPen (Qt::black, 1));

		for (int i = cellSize, k = m_width * cellSize; i < k; i += cellSize)
			pixPainter.drawLine (i, 0, i, pixmap.height ());

		for (int i = cellSize, k = m_height * cellSize; i < k; i += cellSize)
			pixPainter.drawLine (0, i, pixmap.width (), i);
		
		pixPainter.setPen (QPen (Qt::black, 3));
		pixPainter.drawRect (0, 0, pixmap.width (), pixmap.height ());
		
		const Graph& graph = m_table->graph ();
		const Point& lastPoint = m_table->stepQueue ()->lastPoint ();

		const QColor& firstColor = Settings::firstPointColor ();
		const QColor& secondColor = Settings::secondPointColor ();
		
		const QBrush firstBrush (firstColor), secondBrush (secondColor);

		const QPen firstPen (firstColor, 1.5), secondPen (secondColor, 1.5),
				firtBorder (firstColor, 0.5), secondBorder (secondColor, 0.5);
				
		const std::vector<Polygon_ptr>& polygonVector = m_table->polygons ();
		
		const QBrush firstPolyBrush (firstColor,
				BrushComboDelegate::getBrushStyle (Settings::firstFillStyle ()));
		const QBrush secondPolyBrush (secondColor,
				BrushComboDelegate::getBrushStyle (Settings::secondFillStyle ()));
		
		for (Polygon_ptr polygon : polygonVector)
		{
			QPolygon qPoly;
			for (const Point& point : *polygon)
			{
				const Point& newPoint = point + 1;
				qPoly << QPoint (newPoint.x (), newPoint.y ()) * cellSize;
			}
			QPainterPath path;
			path.addPolygon (qPoly);
			pixPainter.fillPath (path, polygon->owner () == FIRST
					? firstPolyBrush
					: secondPolyBrush);
		}
				
		if (!lastPoint.empty ())
		{
			pixPainter.setPen (graph[lastPoint].owner () == FIRST
					? firtBorder
					: secondBorder);
						
			pixPainter.setBrush (Qt::NoBrush);
			const Point& newPoint = lastPoint + 1;
			pixPainter.drawEllipse (QPointF (newPoint.x (), newPoint.y ()) * cellSize, 6, 6);
		}
		
		for (Graph::const_iterator itr = graph.begin (), itrEnd = graph.end ();
				itr != itrEnd; ++itr)
		{
			if (itr->owner () == NONE)
					continue;

			pixPainter.setPen (itr->owner () == FIRST
					? firstPen
					: secondPen);

			pixPainter.setBrush (itr->owner () == FIRST
					? firstBrush
					: secondBrush);
			
			const Point& currPoint = itr.point () + 1;

			pixPainter.drawEllipse (QPointF (currPoint.x (), currPoint.y ()) * cellSize, 3, 3);
				
			const GraphPoint::GraphEdges& edges = itr->edges ();

			for (int j = 0; j < edges.size (); ++j)
			{
				const Point& lastPoint = edges[j] + 1;

				pixPainter.drawLine (QPointF (currPoint.x (), currPoint.y ()) * cellSize,
						QPointF (lastPoint.x (), lastPoint.y ()) * cellSize);
			}
		}
		
		QPainter painter (this);
		const int dx = (rectange.width () - tableWidth) / 2;
		const int dy = (rectange.height () - tableHeight) / 2;
		painter.drawPixmap (dx, dy, pixmap);
	}
}

#include "include/tablewidget.moc"
