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
#include "rival.hpp"

namespace KDots
{
	namespace ipconnect
	{
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_socket (NULL)
		{
		}
		
		Rival::~Rival ()
		{
			if (m_socket)
				m_socket->disconnectFromHost ();
		}
		
		GameConfig Rival::getGameConfig ()
		{
			ClientConfig config;
			
			if (!m_configWidget->clientConfig (config))
			{
				qWarning () << Q_FUNC_INFO;
				return GameConfig ();
			}
			
			//Joining Game
			qDebug () << Q_FUNC_INFO;
			m_socket = new QTcpSocket (this);

			m_socket->connectToHost (config.m_host, config.m_port);

			qDebug () << Q_FUNC_INFO << "Connecting to the server...";
			if (m_socket->waitForConnected ())
			{
				qDebug () << Q_FUNC_INFO << "Connected";
				
				if (m_socket->waitForReadyRead ())
				{
					qDebug () << Q_FUNC_INFO << "Reading Table config";
					QByteArray data = m_socket->readAll();
					qDebug () << Q_FUNC_INFO << "Data size" << data.size ();
					QDataStream in (&data, QIODevice::ReadOnly);
					QVariant variantData;
					in >> variantData;
					if (!variantData.canConvert<GameConfig> ())
					{
						qWarning () << Q_FUNC_INFO << "Cannot convert to GameConfig: " << variantData.typeName ();
					}
					
					const GameConfig& config = variantData.value<GameConfig> ();
					if (!config.isInititialized ())
					{
						qWarning () << Q_FUNC_INFO << "Table config is invalid";
					}
					
					return config;
				}
			}
			
			return GameConfig ();
		}
		
		void Rival::setDotTable (DotTable *table) //Is called after configureWidget
		{
			ServerConfig config;
			if (!m_configWidget->serverConfig (config))
			{
				qWarning () << Q_FUNC_INFO;
				return ;
			}
			
			//Create server
			m_table = table;
			
			qDebug () << Q_FUNC_INFO;
			m_server = new QTcpServer (this);

			connect (m_server,
					SIGNAL (newConnection ()),
					this,
					SLOT (onNewConnectionHandle ()));
			

			if (m_server->listen (QHostAddress::Any, config.m_port))
				qDebug () << Q_FUNC_INFO << "Listening the port" << config.m_port;
			else
				qWarning () << Q_FUNC_INFO << "Couldn't listen the port " << config.m_port;
		}
		
		IConfigurationWidget* Rival::configureWidget ()
		{
			m_configWidget.reset (new ConfigurationWidget);
			ConfigurationWidget *w = m_configWidget.get ();

			return w;
		}

		bool Rival::isAllow () const
		{
			return true;
		}

		void Rival::nextStep (const Point& point)
		{
		}

		void Rival::onNewConnectionHandle ()
		{
			qDebug () << Q_FUNC_INFO;
			m_socket = m_server->nextPendingConnection ();
			
		
			QByteArray gameData;
				
			QDataStream out (&gameData, QIODevice::WriteOnly);
			out << QVariant::fromValue<GameConfig> (m_table->gameConfig ());
			m_socket->write (gameData);
			qDebug () << Q_FUNC_INFO << "Data size" << gameData.size ();
			qDebug () << Q_FUNC_INFO << "Game config sent";
			connect (m_socket,
					SIGNAL (readyRead ()),
					this,
					SLOT (onServerReadyRead ()));

		}

		void Rival::onServerReadyRead ()
		{
			qDebug () << Q_FUNC_INFO;
			//const QByteArray& data = m_socket->readAll();
			//m_socket->write (data);
		}
	}
}