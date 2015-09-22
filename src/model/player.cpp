// -*-c++-*-

/*!
  \file player.cpp
  \brief player data class Source File.
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

#include "player.h"

#include <rcsc/rcg/util.h>

#include <limits>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using rcsc::rcg::Int16;
using rcsc::rcg::UInt16;
using rcsc::rcg::Int32;

namespace {
const float DEG2RADF = 3.14159265358979323846f / 180.0f;
const float RAD2DEGF = 180.0f / 3.14159265358979323846f;
}

/*-------------------------------------------------------------------*/
/*!

*/
Player::Player()
    : M_flag( 0x0000 )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
Player::Player( const rcsc::SideID side,
                const int unum,
                const rcsc::rcg::pos_t & p )
    : M_flag( 0x0000 )
{
    M_data.side_ = ( side == rcsc::LEFT ? 'l'
                     : side == rcsc::RIGHT ? 'r'
                     : 'n' );
    M_data.unum_ = static_cast< Int16 >( unum );
    M_data.type_ = 0;

    M_data.view_quality_ = 'h';

    M_data.focus_side_ = 'n';
    M_data.focus_unum_ = 0;

    M_data.state_ = static_cast< Int32 >( ntohs( p.enable ) );

    M_data.x_ = rcsc::rcg::nstohf( p.x );
    M_data.y_ = rcsc::rcg::nstohf( p.y );
    M_data.vx_ = 0.0f;
    M_data.vy_ = 0.0f;
    M_data.body_ = static_cast< float >( static_cast< Int16 >( ntohs( p.angle ) ) );
    M_data.neck_ = 0.0f;
    M_data.point_x_ = rcsc::rcg::SHOWINFO_SCALE2F;
    M_data.point_y_ = rcsc::rcg::SHOWINFO_SCALE2F;

    M_data.view_width_ = 0.0f;

    M_data.stamina_ = -1.0f;
    M_data.effort_ = 1.0f;
    M_data.recovery_ = 1.0f;

    M_data.kick_count_ = 0;
    M_data.dash_count_ = 0;
    M_data.turn_count_ = 0;
    M_data.catch_count_ = 0;
    M_data.move_count_ = 0;
    M_data.turn_neck_count_ = 0;
    M_data.change_view_count_ = 0;
    M_data.say_count_ = 0;
    M_data.tackle_count_ = 0;
    M_data.pointto_count_ = 0;
    M_data.attentionto_count_ = 0;
}

/*-------------------------------------------------------------------*/
/*!

*/
Player::Player( const rcsc::SideID side,
                const int unum,
                const rcsc::rcg::player_t & p )
    : M_flag( 0x0000 )
{
    M_data.side_ = ( side == rcsc::LEFT ? 'l'
                     : side == rcsc::RIGHT ? 'r'
                     : 'n' );
    M_data.unum_ = static_cast< Int16 >( unum );
    M_data.type_ = static_cast< Int16 >( ntohs( p.type ) );

    M_data.view_quality_ = ntohs( p.view_quality ) ? 'h' : 'l';

    M_data.focus_side_ = 'n';
    M_data.focus_unum_ = 0;

    M_data.state_ = static_cast< Int32 >( ntohs( p.mode ) );

    M_data.x_ = rcsc::rcg::nltohf( p.x );
    M_data.y_ = rcsc::rcg::nltohf( p.y );
    M_data.vx_ = rcsc::rcg::nltohf( p.deltax );
    M_data.vy_ = rcsc::rcg::nltohf( p.deltay );
    M_data.body_ = rcsc::rcg::nltohf( p.body_angle ) * RAD2DEGF;
    M_data.neck_ = rcsc::rcg::nltohf( p.head_angle ) * RAD2DEGF;
    M_data.point_x_ = rcsc::rcg::SHOWINFO_SCALE2F;
    M_data.point_y_ = rcsc::rcg::SHOWINFO_SCALE2F;

    M_data.view_width_ = rcsc::rcg::nltohf( p.view_width ) * RAD2DEGF;

    M_data.stamina_ = rcsc::rcg::nltohf( p.stamina );
    M_data.effort_ = rcsc::rcg::nltohf( p.effort );
    M_data.recovery_ = rcsc::rcg::nltohf( p.recovery );

    M_data.kick_count_ = ntohs( p.kick_count );
    M_data.dash_count_ = ntohs( p.dash_count );
    M_data.turn_count_ = ntohs( p.turn_count );
    M_data.catch_count_ = ntohs( p.catch_count );
    M_data.move_count_ = ntohs( p.move_count );
    M_data.turn_neck_count_ = ntohs( p.turn_neck_count );
    M_data.change_view_count_ = ntohs( p.change_view_count );
    M_data.say_count_ = ntohs( p.say_count );
    M_data.tackle_count_ = 0;
    M_data.pointto_count_ = 0;
    M_data.attentionto_count_ = 0;

    // set flags
    M_flag |= HAVE_TYPE;
    M_flag |= HAVE_DELTA;
    M_flag |= HAVE_STAMINA;
    M_flag |= HAVE_VIEW;
    M_flag |= HAVE_COUNT;
}

/*-------------------------------------------------------------------*/
/*!

*/
Player::Player( const rcsc::rcg::PlayerT & p )
    : M_data( p )
    , M_flag( 0x0000 )
{
    // set flags
    M_flag = 0x0000;
    M_flag |= HAVE_TYPE;
    M_flag |= HAVE_DELTA;
    M_flag |= HAVE_STAMINA;
    M_flag |= HAVE_VIEW;
    M_flag |= HAVE_COUNT;
    M_flag |= HAVE_ARM;
    M_flag |= HAVE_FOCUS;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Player::convertTo( rcsc::rcg::pos_t & to ) const
{
    to.enable = htons( static_cast< Int16 >( M_data.state_ ) );
    to.side = htons( static_cast< Int16 >( M_data.side() ) );
    to.unum = htons( M_data.unum_ );
    to.angle = htons( static_cast< Int16 >( rintf( M_data.body_ ) ) );
    to.x = rcsc::rcg::hftons( M_data.x_ );
    to.y = rcsc::rcg::hftons( M_data.y_ );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Player::convertTo( rcsc::rcg::player_t & to ) const
{
    to.mode = htons( static_cast< Int16 >( M_data.state_ ) );
    to.type = htons( M_data.type_ );

    to.x = rcsc::rcg::hftonl( M_data.x_ );
    to.y = rcsc::rcg::hftonl( M_data.y_ );
    to.deltax = rcsc::rcg::hftonl( M_data.vx_ );
    to.deltay = rcsc::rcg::hftonl( M_data.vy_ );
    to.body_angle = rcsc::rcg::hftonl( M_data.body_ * DEG2RADF );
    to.head_angle = rcsc::rcg::hftonl( M_data.neck_ * DEG2RADF );

    to.view_width = rcsc::rcg::hftonl( M_data.view_width_ * DEG2RADF );
    to.view_quality = htons( M_data.highQuality() ? 1 : 0 );

    to.stamina = rcsc::rcg::hftonl( M_data.stamina_ );
    to.effort = rcsc::rcg::hftonl( M_data.effort_ );
    to.recovery = rcsc::rcg::hftonl( M_data.recovery_ );

    to.kick_count = htons( M_data.kick_count_ );
    to.dash_count = htons( M_data.dash_count_ );
    to.turn_count = htons( M_data.turn_count_ );
    to.say_count = htons( M_data.say_count_ );
    to.turn_neck_count = htons( M_data.turn_neck_count_ );
    to.catch_count = htons( M_data.catch_count_ );
    to.move_count = htons( M_data.move_count_ );
    to.change_view_count = htons( M_data.change_view_count_ );
}
