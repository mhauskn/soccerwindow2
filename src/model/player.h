// -*-c++-*-

/*!
  \file player.h
  \brief player data wrapper class Header File.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifndef SOCCERWINDOW2_MODEL_PLAYER_H
#define SOCCERWINDOW2_MODEL_PLAYER_H

#include <rcsc/types.h>
#include <rcsc/rcg/types.h>

#include <cmath>

/*!
  \class Player
  \brief player info wrapper class
*/
class Player {
private:
    enum {
        HAVE_TYPE    = 0x0001, //!< bit hetero id
        HAVE_DELTA   = HAVE_TYPE << 1, //!< bit speed info
        HAVE_STAMINA = HAVE_TYPE << 2, //!< bit stamina info
        HAVE_VIEW    = HAVE_TYPE << 3, //!< bit view mode & width info
        HAVE_COUNT   = HAVE_TYPE << 4, //!< bit action count info
        HAVE_ARM     = HAVE_TYPE << 5, //!< bit arm info
        HAVE_FOCUS   = HAVE_TYPE << 6, //!< bit focus info
    };

    //! wrappered data
    rcsc::rcg::PlayerT M_data;

    //! information level flag
    int M_flag;

public:

    Player();

    Player( const rcsc::SideID side,
            const int unum,
            const rcsc::rcg::pos_t & p );

    Player( const rcsc::SideID side,
            const int unum,
            const rcsc::rcg::player_t & p );

    explicit
    Player( const rcsc::rcg::PlayerT & p );

    void convertTo( rcsc::rcg::pos_t & to ) const;

    void convertTo( rcsc::rcg::player_t & to ) const;

    // accessor

    const
    rcsc::rcg::PlayerT & data() const
      {
          return M_data;
      }

    rcsc::SideID side() const
      {
          return M_data.side();
      }

    int unum() const
      {
          return M_data.unum_;
      }

    int type() const
      {
          return M_data.type_;
      }

    // information level

    bool hasType() const
      {
          return M_flag & HAVE_TYPE;
      }

    bool hasDelta() const
      {
          return M_flag & HAVE_DELTA;
      }

    bool hasStamina() const
      {
          return M_flag & HAVE_STAMINA;
      }

    bool hasStaminaCapacity() const
      {
          return M_data.stamina_capacity_ >= 0.0f;
      }

    bool hasView() const
      {
          return M_flag & HAVE_VIEW;
      }

    bool hasCommandCount() const
      {
          return M_flag & HAVE_COUNT;
      }

    bool hasArm() const
      {
          return M_flag & HAVE_ARM;
      }

    bool hasFocus() const
      {
          return M_flag & HAVE_FOCUS;
      }


    // player state

    bool isAlive() const
      {
          return M_data.state_ & rcsc::rcg::STAND;
      }

    bool isGoalie() const
      {
          return M_data.state_ & rcsc::rcg::GOALIE;
      }

    bool isKicking() const
      {
          return M_data.state_ & rcsc::rcg::KICK;
      }

    bool isKickingFault() const
      {
          return M_data.state_ & rcsc::rcg::KICK_FAULT;
      }

    bool isCatching() const
      {
          return M_data.state_ & rcsc::rcg::CATCH;
      }

    bool isCatchingFault() const
      {
          return M_data.state_ & rcsc::rcg::CATCH_FAULT;
      }

    bool isCollidedBall() const
      {
          return ( M_data.state_ & rcsc::rcg::BALL_COLLIDE
                   || M_data.state_ & rcsc::rcg::BALL_TO_PLAYER
                   || M_data.state_ & rcsc::rcg::PLAYER_TO_BALL );
      }

    bool isCollidedPlayer() const
      {
          return M_data.state_ & rcsc::rcg::PLAYER_COLLIDE;
      }

    bool isTackling() const
      {
          return M_data.state_ & rcsc::rcg::TACKLE;
      }

    bool isTacklingFault() const
      {
          return M_data.state_ & rcsc::rcg::TACKLE_FAULT;
      }

    bool isPointing() const
      {
          return ( M_data.point_x_ != rcsc::rcg::SHOWINFO_SCALE2F
                   && M_data.point_y_ != rcsc::rcg::SHOWINFO_SCALE2F );
      }

    bool isFocusing() const
      {
          return ( M_data.focus_side_ != 'n'
                   && M_data.focus_unum_ != 0 );
      }

    bool isFoulCharged() const
      {
          return M_data.state_ & rcsc::rcg::FOUL_CHARGED;
      }

    bool hasYellowCard() const
      {
          return M_data.state_ & rcsc::rcg::YELLOW_CARD;
      }

    bool hasRedCard() const
      {
          return M_data.state_ & rcsc::rcg::RED_CARD;
      }

    // position information

    double x() const
      {
          return M_data.x_;
      }

    double y() const
      {
          return M_data.y_;
      }

    double deltaX() const
      {
          return M_data.vx_;
      }

    double deltaY() const
      {
          return M_data.vy_;
      }

    double body() const
      {
          return M_data.body_;
      }

    double head() const
      {
          return M_data.body_ + M_data.neck_;
      }

    // view

    double viewWidth() const
      {
          return M_data.view_width_;
      }

    // stamina

    double stamina() const
      {
          return M_data.stamina_;
      }

    double effort() const
      {
          return M_data.effort_;
      }

    double recovery() const
      {
          return M_data.recovery_;
      }

    double staminaCapacity() const
      {
          return M_data.stamina_capacity_;
      }

    // command count

    int kickCount() const
      {
          return M_data.kick_count_;
      }

    int dashCount() const
      {
          return M_data.dash_count_;
      }

    int turnCount() const
      {
          return M_data.turn_count_;
      }

    int catchCount() const
      {
          return M_data.catch_count_;
      }

    int moveCount() const
      {
          return M_data.move_count_;
      }

    int turnNeckCount() const
      {
          return M_data.turn_neck_count_;
      }

    int changeViewCount() const
      {
          return M_data.change_view_count_;
      }

    int sayCount() const
      {
          return M_data.say_count_;
      }

    int tackleCount() const
      {
          return M_data.tackle_count_;
      }

    int pointtoCount() const
      {
          return M_data.pointto_count_;
      }

    int attentiontoCount() const
      {
          return M_data.attentionto_count_;
      }

    // other flags

    bool hasFullEffort( const double & max_effort ) const
      {
          return std::fabs( max_effort - M_data.effort_ ) < 1.0e-5;
      }

    bool hasFullRecovery() const
      {
          return std::fabs( 1.0 - M_data.recovery_ ) < 1.0e-5;
      }
};

#endif
