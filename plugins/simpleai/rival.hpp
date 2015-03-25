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
#include <interface/irival.hpp>

#include <polygon.hpp>

#include <unordered_set>
#include <QRect>

namespace KDots
{
  class Point;
  namespace simpleai
  {
    class NodeInfo;
    class Rival : public KDots::IRival
    {
      Q_OBJECT
      Q_INTERFACES(KDots::IRival)
    public:
      Rival(QObject *parent = 0);

      Owner owner() const;

      void setBoardModel(BoardModel *board);

    public slots:
      void onPointAdded(const Point& point);
      void onDifficultyChanged(const KgDifficultyLevel *level);

    private:
      void addPoint(bool random = false);
      bool addRandomPoint();

      // Complexity O(n)
      QRect getBoundingBox() const;

      typedef std::vector<float> VectorF;
      typedef std::vector<bool> VectorB;
      // Complexity O(n)
      std::vector<VectorF> getImportanceMatrix(const QRect& bb) const;

      void buildAllowedPointsMap(const QRect& bbox,
                                 const std::unordered_set<Point>& previousPoints,
                                 std::vector<VectorB>& allowedPoints) const;

      // Complexity O(n)
      void findCapturedPoints(const QRect& bbox,
                              const std::vector<VectorB>& allowerdPoints,
                              const PolyList& polygons,
                              std::unordered_set<Point>& capturedPoints) const;

      // Complexity O(1)
      void findPreviousPoints(const std::vector<NodeInfo>& decisionTree,
                              int lastPointID,
                              std::unordered_set<Point>& previousPoints,
                              std::unordered_set<Point>& capturedPoints) const;

    signals:
      void needCreateBoard(const GameConfig& config);
      void needDestroy();
      void needAddPoint(const Point&);

    private:
      BoardModel *m_board;
      Owner m_human, m_ai;
      int m_depth;
      float m_k1;
      float m_k2;
    };
  }
}
