// -*-c++-*-

/*!
  \file view_holder.h
  \brief view data repository Header File.
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

#ifndef SOCCERWINDOW2_MODEL_VIEW_HOLDER_H
#define SOCCERWINDOW2_MODEL_VIEW_HOLDER_H

#include "monitor_view_data.h"
#include "debug_view_data.h"

#include <rcsc/common/player_type.h>
#include <rcsc/common/team_graphic.h>
#include <rcsc/rcg/holder.h>

#include <map>
#include <ostream>

//! drawing data repository
class ViewHolder
    : public rcsc::rcg::Holder {
private:

    //! last changed playmode
    rcsc::PlayMode M_last_playmode;
    //! last changed left team info
    rcsc::rcg::TeamT M_last_team_left;
    //! last changed right team info
    rcsc::rcg::TeamT M_last_team_right;

    //! the record of score change
    std::vector< std::size_t > M_score_change_indexes;

    //! the record of penalty score/miss, first: time, second: playmode
    std::vector< std::pair< int, rcsc::PlayMode > > M_penalty_scores_left;
    std::vector< std::pair< int, rcsc::PlayMode > > M_penalty_scores_right;

    // default type
    rcsc::PlayerType M_default_type;
    //! player_type set
    std::map< int, rcsc::PlayerType > M_player_types;

    //! last received view data
    MonitorViewPtr M_latest_view_data;

    //! view data container.
    std::vector< MonitorViewPtr > M_monitor_view_cont;

    //! debug view data set for left players (key:cycle)
    std::map< int, DebugViewCont > M_left_debug_view;

    //! debug view data set for right players (key:cycle)
    std::map< int, DebugViewCont > M_right_debug_view;


    //! left team graphic
    rcsc::TeamGraphic M_team_graphic_left;

    //! right team graphic
    rcsc::TeamGraphic M_team_graphic_right;

    //! nocopyable
    ViewHolder( const ViewHolder & );
    //! nocopyable
    ViewHolder & operator=( const ViewHolder & );

public:
    //! constructor.
    ViewHolder();

    ~ViewHolder();
    //--------------------------------------------------
    //! clear all stored data
    void clear();

    //! save a stored rcg data
    std::ostream & saveRCG( std::ostream & os ) const;

    //! save debug view data to directory
    void saveDebugView( const std::string & dir_path ) const;

    //! load debug view data from directory
    void openDebugView( const std::string & dir_path );

    //--------------------------------------------------
    // override methods.

    // rcg v3 or older

    //! register showinfo_t (rcg v2, monitor v1)
    bool addShowInfo( const rcsc::rcg::showinfo_t & show );
    //! register showinfo_t (monitor v2 only)
    bool addShowInfo2( const rcsc::rcg::showinfo_t2 & show2 );
    //! register showinfo_t (rcg v3 only)
    bool addShortShowInfo2( const rcsc::rcg::short_showinfo_t2 & show2 );
    bool addMsgInfo( const rcsc::rcg::Int16 board,
                     const std::string & msg );
    bool addDrawInfo( const rcsc::rcg::drawinfo_t & draw );
    bool addPlayMode( const char pmode );
    bool addTeamInfo( const rcsc::rcg::team_t & team_l,
                      const rcsc::rcg::team_t & team_r );
    bool addServerParam( const rcsc::rcg::server_params_t & sparams );
    bool addPlayerParam( const rcsc::rcg::player_params_t & pparams );
    bool addPlayerType( const rcsc::rcg::player_type_t & ptinfo );

    // monitor v3 & rcg v4

    bool addShow( const int time,
                  const rcsc::rcg::ShowInfoT & show );
    bool addMsg( const int time,
                 const int board,
                 const std::string & msg );
    bool addPlayMode( const int time,
                      const rcsc::PlayMode pm );
    bool addTeam( const int time,
                  const rcsc::rcg::TeamT & team_l,
                  const rcsc::rcg::TeamT & team_r );
    bool addServerParam( const std::string & msg );
    bool addPlayerParam( const std::string & msg );
    bool addPlayerType( const std::string & msg );

    //--------------------------------------------------

    void pushBackLatestViewData();

    //! set debug view data
    void addDebugView( const int cycle,
                       const char side,
                       const int unum,
                       DebugViewPtr debug_view );

    //--------------------------------------------------

    rcsc::PlayMode lastPlayMode() const
      {
          return M_last_playmode;
      }

    //! get const reference of data container.
    const
    std::vector< MonitorViewPtr > & monitorViewCont() const
      {
          return M_monitor_view_cont;
      }

    //! get debug view data container
    const
    std::map< int, DebugViewCont > & leftDebugView() const
      {
          return M_left_debug_view;
      }
    //! get debug view data container
    const
    std::map< int, DebugViewCont > & rightDebugView() const
      {
          return M_right_debug_view;
      }

    const
    std::map< int, rcsc::PlayerType > & playerTypeCont() const
      {
          return M_player_types;
      }

    const
    rcsc::PlayerType & playerType( const int id ) const;

    const
    std::vector< std::size_t > & scoreChangeIndexes() const
      {
          return M_score_change_indexes;
      }

    const
    std::vector< std::pair< int, rcsc::PlayMode > > & penaltyScoresLeft() const
      {
          return M_penalty_scores_left;
      }

    const
    std::vector< std::pair< int, rcsc::PlayMode > > & penaltyScoresRight() const
      {
          return M_penalty_scores_right;
      }

    MonitorViewConstPtr getViewData( const std::size_t idx ) const;

    MonitorViewConstPtr latestViewData() const
      {
          return M_latest_view_data;
      }

    std::size_t getIndexOf( const int cycle ) const;

    const
    rcsc::TeamGraphic & teamGraphicLeft() const
      {
          return M_team_graphic_left;
      }

    const
    rcsc::TeamGraphic & teamGraphicRight() const
      {
          return M_team_graphic_right;
      }

private:
    bool saveDebugView( const std::string & dir_path,
                        const std::string & team_name,
                        const int unum,
                        const std::map< int, DebugViewCont > & data ) const;

    bool openDebugView( const std::string & dir_path,
                        const std::string & team_name,
                        const int unum );

    void addMonitorViewData( MonitorViewPtr ptr );

    bool analyzeTeamGraphic( const std::string & msg );
};

#endif
