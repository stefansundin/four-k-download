/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/


#ifndef MULTIMEDIA_PLAYER_H
#define MULTIMEDIA_PLAYER_H

#include <QObject>
#include <QMetaType>

namespace Multimedia
{

class Player : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_ENUMS(State)

public:
    enum State { StopState, PlayState, PauseState };

    explicit Player(QObject* parent = 0);

    State state() const;

public slots:
    virtual void play() = 0;
    virtual void pause() = 0;

signals:
    void stateChanged(Multimedia::Player::State value);

protected:
    void setState(State value);

private:
    State m_state;
};

} // Multimedia

Q_DECLARE_METATYPE(Multimedia::Player::State)

#endif // MULTIMEDIA_PLAYER_H
