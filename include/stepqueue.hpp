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
#ifndef KDOTS_STEPQUEUE_HPP
#define KDOTS_STEPQUEUE_HPP
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{
	class KDOTS_EXPORT StepQueue
	{
		Owner m_firstOwner;
		std::vector<Point> m_firstPoints, m_secondPoints, m_points;
		int m_first, m_second;
	protected:
		Owner m_owner;
		bool m_captured;
	public:
		StepQueue (Owner firstPlayer);
		virtual ~StepQueue () {}
		
		inline Owner firstOwner () const
		{
			return m_firstOwner;
		}
		
		inline Point lastPoint () const
		{
			return m_points.empty () ? Point () : m_points.back ();
		}
		
		inline void clear ()
		{
			m_first = m_second = 0;
			m_owner  = m_firstOwner;
			m_captured  = false;
			m_firstPoints.clear ();
			m_secondPoints.clear ();
			m_points.clear ();
		}

		void addPoint (const Point& point);
		void addCaptured ();

		inline Owner getCurrentOwner () const
		{
			return m_owner;
		}

		inline int getMarks (Owner owner) const
		{
			return owner == FIRST ? m_first : m_second;
		}

		inline std::vector<Point> getPoints (Owner owner) const
		{
			return owner == SECOND ? m_secondPoints : m_firstPoints;
		}
		
		inline std::vector<Point> getAllPoints () const
		{
			return m_points;
		}

		inline static Owner other (Owner player)
		{
			return player == FIRST ? SECOND : FIRST;
		}

		virtual Owner nextStep ()
		{
			m_captured = false;
			return (m_owner = other (m_owner));
		}
	};

	class ExtraStepQueue : public StepQueue
	{
	public:
		ExtraStepQueue (Owner firstPlayer);

		Owner
		nextStep ()
		{
			if (m_captured)
				return m_owner;

			m_captured = false;

			return (m_owner = other (m_owner));
		}
	};


}

#endif