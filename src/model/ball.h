// -*-c++-*-

/*!
  \file ball.h
  \brief ball data wrapper class Header File.
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

#ifndef SOCCERWINDOW2_MODEL_BALL_H
#define SOCCERWINDOW2_MODEL_BALL_H

#include <rcsc/rcg/types.h>

/*!
  \class Ball
  \brief ball info wrapper class

  This class contains ball info by RCG data format.
  But ball_t has local byte order variables.
*/
class Ball {
private:
    rcsc::rcg::BallT M_data; //!< wrappered data
    bool M_have_delta; //!< true if ball has velocity info.
public:

    Ball();

    explicit
    Ball( const rcsc::rcg::pos_t & p );

    explicit
    Ball( const rcsc::rcg::ball_t & ball );

    explicit
    Ball( const rcsc::rcg::BallT & ball );

    void convertTo( rcsc::rcg::pos_t & to ) const;

    void convertTo( rcsc::rcg::ball_t & to ) const;

    // accessor

    const
    rcsc::rcg::BallT & data() const
      {
          return M_data;
      }

    // information level

    bool hasDelta() const
      {
          return M_have_delta;
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
};

#endif
