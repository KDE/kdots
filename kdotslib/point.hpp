/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <QMetaType>

#include "constants.hpp"

class QPointF;
class QPoint;

namespace KDots
{
  class KDOTS_EXPORT Point final
  {
  public:
    Point();
    Point(int x, int y);
    
    Point operator*(int val) const;
    
    Point operator+(int val) const;
    
    Point operator-(const Point& point) const;
    
    Point operator+(const Point& point) const;
    
    bool operator<(const Point& other) const;
    bool operator<=(const Point& other) const;
    
    bool operator>(const Point& other) const;
    bool operator>=(const Point& other) const;

    bool operator==(const Point& a) const;
    bool operator!=(const Point& a) const;

    bool empty() const;
    
    friend QDataStream& operator<<(QDataStream& out, const Point& obj);
    friend QDataStream& operator>>(QDataStream& in, Point& obj);
    
    explicit operator QPointF() const;
    explicit operator QPoint() const;
    
  public:
    int m_x, m_y;
  };
  
  QDataStream& operator<<(QDataStream& out, const Point& obj);

  QDataStream& operator>>(QDataStream& in, Point& obj);
}

namespace std
{
  template<>
  struct hash<KDots::Point>
  {
    std::size_t operator()(const KDots::Point& s) const;
  };
}

Q_DECLARE_METATYPE(KDots::Point);