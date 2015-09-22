// -*-c++-*-

/*!
  \file monitor view_data.cpp
  \brief monitor view data type for drawing on soccerwindow Source File.
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

#include "monitor_view_data.h"

/*-------------------------------------------------------------------*/
/*!

*/
MonitorViewData::MonitorViewData( const rcsc::rcg::ShowInfoT & show,
                                  const rcsc::PlayMode pmode,
                                  const rcsc::rcg::TeamT & team_l,
                                  const rcsc::rcg::TeamT & team_r )
    : M_playmode( pmode )
    , M_ball( show.ball_ )
    , M_cycle( show.time_  )
{
    M_teams[0].assign( team_l );
    M_teams[1].assign( team_r );

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER*2 ; ++i )
    {
        M_players.push_back( Player( show.player_[i] ) );
    }
}

/*-------------------------------------------------------------------*/
/*!
  \param show RCG v3 format view data
  \param pmode RCG format playmode data
  \param team_l left team data
  \param team_r right team data
*/
MonitorViewData::MonitorViewData( const rcsc::rcg::short_showinfo_t2 & show,
                                  const rcsc::PlayMode pmode,
                                  const rcsc::rcg::TeamT & team_l,
                                  const rcsc::rcg::TeamT & team_r )
    : M_playmode( pmode )
    , M_ball( show.ball )
    , M_cycle( static_cast< int >( ntohs( show.time ) ) )
{
    if ( M_cycle == 65535 ) M_cycle = 0;

    M_teams[0].assign( team_l );
    M_teams[1].assign( team_r );

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER ; ++i )
    {
        M_players.push_back( Player( rcsc::LEFT,
                                     i + 1,
                                     show.pos[i] ) );
    }
    for ( int i = rcsc::MAX_PLAYER; i < rcsc::MAX_PLAYER * 2 ; ++i )
    {
        M_players.push_back( Player( rcsc::RIGHT,
                                     i - rcsc::MAX_PLAYER + 1,
                                     show.pos[i] ) );
    }
}

/*-------------------------------------------------------------------*/
/*!
  \param show monitor protocol v2 view data
*/
MonitorViewData::MonitorViewData( const rcsc::rcg::showinfo_t2 & show )
    : M_playmode( show.pmode )
    , M_ball( show.ball )
    , M_cycle( static_cast< int >( ntohs( show.time ) ) )
{
    if ( M_cycle == 65535 ) M_cycle = 0;

    M_teams[0].assign( show.team[0] );
    M_teams[1].assign( show.team[1] );

    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 0; i < rcsc::MAX_PLAYER; ++i )
    {
        M_players.push_back( Player( rcsc::LEFT,
                                     i + 1,
                                     show.pos[i] ) );
    }
    for ( int i = rcsc::MAX_PLAYER;
          i < rcsc::MAX_PLAYER * 2;
          ++i )
    {
        M_players.push_back( Player( rcsc::RIGHT,
                                     i - rcsc::MAX_PLAYER + 1,
                                     show.pos[i] ) );
    }
}

/*-------------------------------------------------------------------*/
/*!
  \param show monitor v1 or RCG v1,v2 view data
*/
MonitorViewData::MonitorViewData( const rcsc::rcg::showinfo_t & show )
    : M_playmode( show.pmode )
    , M_ball( show.pos[0] )
    , M_cycle( static_cast< int >( ntohs( show.time ) ) )
{
    if ( M_cycle == 65535 ) M_cycle = 0;

    M_teams[0].assign( show.team[0] );
    M_teams[1].assign( show.team[1] );

    // In old format, players' index is started from '1'.
    M_players.reserve( rcsc::MAX_PLAYER * 2 );
    for ( int i = 1; i < rcsc::MAX_PLAYER + 1; ++i )
    {
        M_players.push_back( Player( rcsc::LEFT,
                                     i,
                                     show.pos[i] ) );
    }
    for ( int i = rcsc::MAX_PLAYER + 1;
          i < rcsc::MAX_PLAYER * 2 + 1;
          ++i )
    {
        M_players.push_back( Player( rcsc::RIGHT,
                                     i - rcsc::MAX_PLAYER,
                                     show.pos[i] ) );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorViewData::convertTo( rcsc::rcg::showinfo_t2 & show2 ) const
{
    show2.pmode = static_cast< char >( playmode().mode() );

    leftTeam().convertTo( show2.team[0] );
    rightTeam().convertTo( show2.team[1] );

    ball().convertTo( show2.ball );

    int i = 0;
    const std::vector< Player >::const_iterator end = players().end();
    for ( std::vector< Player >::const_iterator p = players().begin();
          p != end;
          ++p, ++i )
    {
        p->convertTo( show2.pos[i] );
    }

    show2.time = htons( static_cast< short >( cycle() ) );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
MonitorViewData::convertTo( rcsc::rcg::DispInfoT & disp ) const
{
    disp.pmode_ = playmode().mode();

    disp.team_[0] = leftTeam().data();
    disp.team_[1] = rightTeam().data();

    disp.show_.ball_ = ball().data();

    int i = 0;
    const std::vector< Player >::const_iterator end = players().end();
    for ( std::vector< Player >::const_iterator p = players().begin();
          p != end;
          ++p, ++i )
    {
        disp.show_.player_[i] = p->data();
    }

    disp.show_.time_ = cycle();
}
