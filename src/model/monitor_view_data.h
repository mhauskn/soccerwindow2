// -*-c++-*-

/*!
  \file monitor_view_data.h
  \brief monitor view data type for drawing on soccerwindow Header File.
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

#ifndef SOCCERWINDOW2_MONITOR_VIEW_DATA_H
#define SOCCERWINDOW2_MONITOR_VIEW_DATA_H

#include "team.h"
#include "ball.h"
#include "player.h"
#include "playmode.h"

#include <rcsc/rcg/types.h>

#include <boost/shared_ptr.hpp>
#include <vector>

//! one cycle monitor data snapshot
class MonitorViewData {
private:
    PlayMode M_playmode;
    Team M_teams[2];
    Ball M_ball;
    std::vector< Player > M_players;
    int M_cycle; //!< current game cycle

    //! not used
    MonitorViewData();
    //! not used
    MonitorViewData( const MonitorViewData & );
    //! not used
    MonitorViewData & operator=( const MonitorViewData & );
public:

    //! construct from rcg v4 data (only for rcg v4)
    MonitorViewData( const rcsc::rcg::ShowInfoT & show,
                     const rcsc::PlayMode pmode,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    //! construct from rcg v3 data (only for rcg v3)
    MonitorViewData( const rcsc::rcg::short_showinfo_t2 & show,
                     const rcsc::PlayMode pmode,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    //! construct from rcg v3 data (only for monitor v2)
    MonitorViewData( const rcsc::rcg::showinfo_t2 & show );

    //! construct from rcg v1 data
    MonitorViewData( const rcsc::rcg::showinfo_t & show );


    /*!
      \brief convert to the monitor protocol data
     */
    void convertTo( rcsc::rcg::showinfo_t2 & show2 ) const;

    /*!
      \brief convert to the monitor protocol data
     */
    void convertTo( rcsc::rcg::DispInfoT & disp ) const;

    /*!
      \brief returns playmode info
      \return const reference to the playmode data object
    */
    const
    PlayMode & playmode() const
      {
          return M_playmode;
      }

    /*!
      \brief returns left team info
      \return const reference to the left team data object
    */
    const
    Team & leftTeam() const
      {
          return M_teams[0];
      }

    /*!
      \brief returns right team info
      \return const reference to the right team data object
    */
    const
    Team & rightTeam() const
      {
          return M_teams[1];
      }

    /*!
      \brief returns ball info
      \return const reference to the ball data object
    */
    const
    Ball & ball() const
      {
          return M_ball;
      }

    /*!
      \brief returns all players info
      \return const reference to the player container
    */
    const
    std::vector< Player > & players() const
      {
          return M_players;
      }

    /*!
      \brief returns game cycle info
      \return cycle value
    */
    int cycle() const
      {
          return M_cycle;
      }

};


// type definitin for convinience
typedef boost::shared_ptr< MonitorViewData > MonitorViewPtr;
typedef boost::shared_ptr< const MonitorViewData > MonitorViewConstPtr;
typedef std::vector< MonitorViewPtr > MonitorViewCont;

#endif
