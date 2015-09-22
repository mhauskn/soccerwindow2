// -*-c++-*-

/*!
  \file ball.cpp
  \brief ball data class Source File.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include "ball.h"

#include <rcsc/rcg/types.h>
#include <rcsc/rcg/util.h>

#include <cmath>

/*-------------------------------------------------------------------*/
/*!

*/
Ball::Ball()
    : M_have_delta( false )
{
    M_data.x_ = 0.0f;
    M_data.y_ = 0.0f;
    M_data.vx_ = 0.0f;
    M_data.vy_ = 0.0f;
}

/*-------------------------------------------------------------------*/
/*!

*/
Ball::Ball( const rcsc::rcg::pos_t & ball )
    : M_have_delta( false )
{
    M_data.x_ = rcsc::rcg::nstohd( ball.x );
    M_data.y_ = rcsc::rcg::nstohd( ball.y );
    M_data.vx_ = 0.0f;
    M_data.vy_ = 0.0f;
}

/*-------------------------------------------------------------------*/
/*!

*/
Ball::Ball( const rcsc::rcg::ball_t & ball )
    : M_have_delta( true )
{
    M_data.x_ = rcsc::rcg::nltohf( ball.x );
    M_data.y_ = rcsc::rcg::nltohf( ball.y );
    M_data.vx_ = rcsc::rcg::nltohf( ball.deltax );
    M_data.vy_ = rcsc::rcg::nltohf( ball.deltay );
}

/*-------------------------------------------------------------------*/
/*!

*/
Ball::Ball( const rcsc::rcg::BallT & ball )
    : M_data( ball )
    , M_have_delta( true )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
Ball::convertTo( rcsc::rcg::pos_t & to ) const
{
    to.x = rcsc::rcg::hftons( M_data.x_ );
    to.y = rcsc::rcg::hftons( M_data.y_ );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Ball::convertTo( rcsc::rcg::ball_t & to ) const
{
    to.x = rcsc::rcg::hftonl( M_data.x_ );
    to.y = rcsc::rcg::hftonl( M_data.y_ );
    to.deltax = rcsc::rcg::hftonl( M_data.vx_ );
    to.deltay = rcsc::rcg::hftonl( M_data.vy_ );
}
