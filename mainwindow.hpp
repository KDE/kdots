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
#include <memory>
#include <KXmlGuiWindow>
#include <KgDifficulty>
#include "constants.hpp"

namespace Ui
{
  class MainWindow;
}

class KAction;

namespace KDots
{
  class BoardModel;
  class IRival;

  class MainWindow : public KXmlGuiWindow
  {
    Q_OBJECT
  public:
    MainWindow(QWidget *parent = 0);

  private:
    void initMenu();

  private slots:
    void onNewGame();
    void onPreferences();
    void endGame();
    void onNeedDestroy();
  
  signals:
    void preferencesUpdated();

  private:
    Ui::MainWindow *m_ui;
    std::unique_ptr<BoardModel> m_model;
    bool m_needDestroy;
    bool m_gameConfiguring;
    
    struct
    {
      KAction *m_newAction;
      KAction *m_endAction;
      KAction *m_quitAction;
      KAction *m_undoAction;
    } m_menu;
  };
}