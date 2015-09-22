// -*-c++-*-

/*!
  \file view_holder.cpp
  \brief view data repository Source File.
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

#include "view_holder.h"

#include "options.h"
#include "debug_client_parser.h"

#include <rcsc/common/player_param.h>
#include <rcsc/common/server_param.h>
#include <rcsc/rcg/serializer.h>

#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstring>

const std::size_t RCG_SIZE_LIMIT = 6000 * 8;

const std::string DEFAULT_DEBUG_VIEW_EXTENSION = ".dcl";

/*-------------------------------------------------------------------*/
/*!
  default constructor.
*/
ViewHolder::ViewHolder()
    : M_last_playmode( rcsc::PM_Null )
    , M_default_type()
{
    M_player_types.insert( std::pair< int, rcsc::PlayerType >( 0, M_default_type ) );

    M_monitor_view_cont.reserve( 1024 * 8 );
}

/*-------------------------------------------------------------------*/
/*!

*/
ViewHolder::~ViewHolder()
{
    //std::cerr << "ViewHolder Left Debug View Size " << M_left_debug_view.size() << std::endl;
    //std::cerr << "ViewHolder Right Debug View Size " << M_right_debug_view.size() << std::endl;
}

/*-------------------------------------------------------------------*/
/*!
  clear all stored data
*/
void
ViewHolder::clear()
{
    M_last_playmode = rcsc::PM_Null;

    M_last_team_left.clear();
    M_last_team_right.clear();

    M_score_change_indexes.clear();
    M_penalty_scores_left.clear();
    M_penalty_scores_right.clear();

    M_latest_view_data.reset();
    M_monitor_view_cont.clear();

    M_left_debug_view.clear();
    M_right_debug_view.clear();

    M_default_type = rcsc::PlayerType();
    M_player_types.clear();
    M_player_types.insert( std::pair< int, rcsc::PlayerType >( 0, M_default_type ) );

    M_team_graphic_left.clear();
    M_team_graphic_right.clear();
}

/*-------------------------------------------------------------------*/
/*!

*/
std::ostream &
ViewHolder::saveRCG( std::ostream & os ) const
{
    rcsc::rcg::Serializer::Ptr serializer = rcsc::rcg::Serializer::create( logVersion() );

    if ( ! serializer )
    {
        std::cerr << "Failed to create the RCG serializer version " << logVersion()
                  << std::endl;
        return os;
    }

    // header
    serializer->serializeHeader( os );

    // params

    serializer->serializeParam( os, rcsc::ServerParam::i().toStr() );
    serializer->serializeParam( os, rcsc::PlayerParam::i().toStr() );
    for ( std::map< int, rcsc::PlayerType >::const_iterator it
              = playerTypeCont().begin();
          it != playerTypeCont().end();
          ++it )
    {
        serializer->serializeParam( os, it->second.toStr() );
    }

    // playmode, team, show

    rcsc::rcg::DispInfoT disp;

    const std::vector< MonitorViewPtr >::const_iterator end
        = monitorViewCont().end();
    for ( std::vector< MonitorViewPtr >::const_iterator it
              = monitorViewCont().begin();
          it != end;
          ++it )
    {
        (*it)->convertTo( disp );

        serializer->serialize( os, disp );
    }

//     rcsc::rcg::dispinfo_t2 disp2;

//     // server param
//     rcsc::ServerParam::i().convertTo( disp2.body.sparams );
//     serializer->serialize( os, disp2.body.sparams );

//     // player param
//     rcsc::PlayerParam::i().convertTo( disp2.body.pparams );
//     serializer->serialize( os, disp2.body.pparams );

//     // player types
//     for ( std::map< int, rcsc::PlayerType >::const_iterator it
//               = playerTypeCont().begin();
//           it != playerTypeCont().end();
//           ++it )
//     {
//         it->second.convertTo( disp2.body.ptinfo );
//         serializer->serialize( os, disp2.body.ptinfo );
//     }

//     // view data, team data, playmode ...
//     const std::vector< MonitorViewPtr >::const_iterator end
//         = monitorViewCont().end();
//     for ( std::vector< MonitorViewPtr >::const_iterator it
//               = monitorViewCont().begin();
//           it != end;
//           ++it )
//     {
//         (*it)->convertTo( disp2.body.show );

//         serializer->serialize( os, disp2.body.show );
//     }

    return os;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::saveDebugView( const std::string & dir_path ) const
{
    if ( dir_path.empty() )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " Dir path string is empty!"
                  << std::endl;
        return;
    }

    if ( ! M_latest_view_data )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " No view data!!"
                  << std::endl;
        return;
    }

    std::string left_team = M_latest_view_data->leftTeam().name();

    if ( ! left_team.empty()
         && ! M_left_debug_view.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( saveDebugView( dir_path, left_team, unum,
                                M_left_debug_view ) )
            {
                std::cerr << "saved debug view data : "
                          << left_team << "-" << unum
                          << std::endl;
            }
        }
    }

    std::string right_team = M_latest_view_data->rightTeam().name();

    if ( ! right_team.empty()
         && ! M_right_debug_view.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( saveDebugView( dir_path, right_team, unum,
                                M_right_debug_view ) )
            {
                std::cerr << "saved debug view data : "
                          << right_team << "-" << unum
                          << std::endl;
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::saveDebugView( const std::string & dir_path,
                           const std::string & team_name,
                           const int unum,
                           const std::map< int, DebugViewCont > & data ) const
{
    std::string filepath = dir_path;
    {
        if ( filepath.empty()
             || *filepath.rbegin() != '/' )
        {
            filepath += '/';
        }

        std::ostringstream ostr;
        ostr << team_name << '-' << unum
             << DEFAULT_DEBUG_VIEW_EXTENSION;
        ostr << std::flush;
        filepath += ostr.str();
    }

    std::ofstream fout( filepath.c_str() );

    if ( ! fout )
    {
        return false;
    }

    const int last_cycle = M_latest_view_data->cycle();

    for ( int i = 0; i < last_cycle; ++i )
    {
        std::map< int, DebugViewCont >::const_iterator it = data.find( i );
        if ( it != data.end()
             && it->second[unum - 1] )
        {
            it->second[unum - 1]->print( fout, i );
        }
    }

    fout.close();

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::openDebugView( const std::string & dir_path )
{
    if ( dir_path.empty() )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " Dir path string is empty!"
                  << std::endl;
        return;
    }

    if ( ! M_latest_view_data )
    {
        std::cerr << __FILE__ << ':' << __LINE__
                  << " No view data!!"
                  << std::endl;
        return;
    }

    std::string left_team = M_latest_view_data->leftTeam().name();

    if ( ! left_team.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( openDebugView( dir_path, left_team, unum ) )
            {
                std::cerr << "read debug view data : "
                          << left_team << "-" << unum
                          << std::endl;
            }
        }
    }

    std::string right_team = M_latest_view_data->rightTeam().name();

    if ( ! right_team.empty() )
    {
        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( openDebugView( dir_path, right_team, unum ) )
            {
                std::cerr << "read debug view data : "
                          << right_team << "-" << unum
                          << std::endl;
            }
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::openDebugView( const std::string & dir_path,
                           const std::string & team_name,
                           const int unum )
{
    std::string filepath = dir_path;
    {
        if ( filepath.empty()
             || *filepath.rbegin() != '/' )
        {
            filepath += '/';
        }

        std::ostringstream ostr;
        ostr << team_name << '-' << unum
             << DEFAULT_DEBUG_VIEW_EXTENSION;
        ostr << std::flush;
        filepath += ostr.str();
    }

    std::ifstream fin( filepath.c_str() );

    if ( ! fin )
    {
        return false;
    }

    DebugClientParser parser;

    std::string line_buf;
    while ( std::getline( fin, line_buf ) )
    {
        if ( line_buf.compare( 0, 10, "%% debug [" ) )
        {
            // other debug log data
            continue;
        }

        parser.parse( line_buf.c_str() + 10, *this );
    }

    fin.close();
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addShowInfo( const rcsc::rcg::showinfo_t & show )
{
    if ( M_monitor_view_cont.size() > RCG_SIZE_LIMIT )
    {
        return false;
    }

    MonitorViewPtr ptr( new MonitorViewData( show ) );

    addPlayMode( show.pmode );
    addTeamInfo( show.team[0], show.team[1] );

    addMonitorViewData( ptr );
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addShowInfo2( const rcsc::rcg::showinfo_t2 & show2 )
{
    if ( M_monitor_view_cont.size() > RCG_SIZE_LIMIT )
    {
        return false;
    }

    MonitorViewPtr ptr( new MonitorViewData( show2 ) );

    addPlayMode( show2.pmode );
    addTeamInfo( show2.team[0], show2.team[1] );

    addMonitorViewData( ptr );
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addShortShowInfo2( const rcsc::rcg::short_showinfo_t2 & show2 )
{
    if ( M_monitor_view_cont.size() > RCG_SIZE_LIMIT )
    {
        return false;
    }

    MonitorViewPtr ptr( new MonitorViewData( show2,
                                             M_last_playmode,
                                             M_last_team_left,
                                             M_last_team_right ) );
    addMonitorViewData( ptr );
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addMsgInfo( const rcsc::rcg::Int16,
                        const std::string & msg )
{
    //std::cerr << "ViewHolder::addMsgInfo" << std::endl;
    //std::cout << "recv msginfo_t: message = [" << msg << ']'
    //          << std::endl;

    //(team_graphic_l (6 0 "8 8 2 1" "@    c #FF5D5D" "#   c #FF0000" "@#######" "########" "########" "########" "########" "########" "########" "########")

    if ( ! std::strncmp( msg.c_str(),
                         "(team_graphic_",
                         std::strlen( "(team_graphic_" ) ) )
    {
        return analyzeTeamGraphic( msg );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addDrawInfo( const rcsc::rcg::drawinfo_t & msg )
{
    std::cout << "recv drawinfo_t: mode = " << msg.mode
              << std::endl;
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addPlayMode( const char pmode )
{
    rcsc::PlayMode playmode = static_cast< rcsc::PlayMode >( pmode );

    if ( M_last_playmode != playmode )
    {
        if ( playmode == rcsc::PM_PenaltyScore_Left
             || playmode == rcsc::PM_PenaltyMiss_Left )
        {
            int cycle = ( M_latest_view_data
                          ? M_latest_view_data->cycle()
                          : 0 );
            M_penalty_scores_left.push_back( std::make_pair( cycle, playmode ) );
        }
        else if ( playmode == rcsc::PM_PenaltyScore_Right
                  || playmode == rcsc::PM_PenaltyMiss_Right )
        {
            int cycle = ( M_latest_view_data
                          ? M_latest_view_data->cycle()
                          : 0 );
            M_penalty_scores_right.push_back( std::make_pair( cycle, playmode ) );
        }
    }

    M_last_playmode = playmode;
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  push back new data
*/
bool
ViewHolder::addTeamInfo( const rcsc::rcg::team_t & team_l,
                         const rcsc::rcg::team_t & team_r )
{
    rcsc::rcg::UInt16 score_l = static_cast< rcsc::rcg::UInt16 >( ntohs( team_l.score ) );
    rcsc::rcg::UInt16 score_r = static_cast< rcsc::rcg::UInt16 >( ntohs( team_r.score ) );

    if ( M_last_team_left.score_ != score_l
         || M_last_team_right.score_ != score_r )
    {
        M_score_change_indexes.push_back( M_monitor_view_cont.size() );
    }

    char buf[18];

    std::memset( buf, '\0', 18 );
    std::strncpy( buf, team_l.name, 16 );
    M_last_team_left.name_ = buf;
    M_last_team_left.score_ = score_l;

    std::memset( buf, '\0', 18 );
    std::strncpy( buf, team_r.name, 16 );
    M_last_team_right.name_ = team_r.name;
    M_last_team_right.score_ = score_r;

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  add new hetero player param
*/
bool
ViewHolder::addPlayerType( const rcsc::rcg::player_type_t & param )
{
    //std::cerr << "ViewHolder::addPlayerType" << std::endl;

    int id = static_cast< int >( htons( param.id ) );
    if ( id < 0 )
    {
        std::cerr << "ViewHolder::addPlayerType(). illegal player type id" << std::endl;
        return false;
    }

    if ( M_player_types.find( id ) != M_player_types.end() )
    {
        M_player_types[id] = rcsc::PlayerType( param );
    }
    else
    {
        M_player_types.insert( std::pair< int, rcsc::PlayerType >
                               ( id, rcsc::PlayerType( param ) ) );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  set server param
*/
bool
ViewHolder::addServerParam( const rcsc::rcg::server_params_t & param )
{
    //std::cerr << "ViewHolder::addServerParam" << std::endl;
    rcsc::ServerParam::instance().convertFrom( param );
    M_default_type = rcsc::PlayerType();
    return true;
}

/*-------------------------------------------------------------------*/
/*!
  set player param
*/
bool
ViewHolder::addPlayerParam( const rcsc::rcg::player_params_t & param )
{
    //std::cerr << "ViewHolder::addPlayerParam" << std::endl;
    rcsc::PlayerParam::instance().convertFrom( param );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addShow( const int,
                     const rcsc::rcg::ShowInfoT & show )
{
    if ( M_monitor_view_cont.size() > RCG_SIZE_LIMIT )
    {
        return false;
    }

    MonitorViewPtr ptr( new MonitorViewData( show,
                                             M_last_playmode,
                                             M_last_team_left,
                                             M_last_team_right ) );
    addMonitorViewData( ptr );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addMsg( const int,
                    const int,
                    const std::string & msg )
{
    if ( ! msg.compare( 0, std::strlen( "(team_graphic_" ),
                        "(team_graphic_" ) )
    {
        return analyzeTeamGraphic( msg );
    }
    else if ( ! msg.compare( 0, std::strlen( "(change_player_type" ),
                             "(change_player_type" ) )

    {
        return true;
    }

    std::cerr << "ViewHolder::addMsg [" << msg << "]" << std::endl;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addPlayMode( const int,
                         const rcsc::PlayMode pm )
{
    //std::cerr << "ViewHolder::addPlayMode " << time << " " << pm << std::endl;
    if ( M_last_playmode != pm )
    {
        if ( pm == rcsc::PM_PenaltyScore_Left
             || pm == rcsc::PM_PenaltyMiss_Left )
        {
            int cycle = ( M_latest_view_data
                          ? M_latest_view_data->cycle()
                          : 0 );
            M_penalty_scores_left.push_back( std::make_pair( cycle, pm ) );
        }
        else if ( pm == rcsc::PM_PenaltyScore_Right
                  || pm == rcsc::PM_PenaltyMiss_Right )
        {
            int cycle = ( M_latest_view_data
                          ? M_latest_view_data->cycle()
                          : 0 );
            M_penalty_scores_right.push_back( std::make_pair( cycle, pm ) );
        }
    }

    M_last_playmode = pm;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addTeam( const int,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r )
{
    if ( M_last_team_left.score_ != team_l.score_
         || M_last_team_right.score_ != team_r.score_ )
    {
        M_score_change_indexes.push_back( M_monitor_view_cont.size() );
    }

    M_last_team_left = team_l;
    M_last_team_right = team_r;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addServerParam( const std::string & msg )
{
    //std::cerr << "ViewHolder::addServerParam" << std::endl;
    rcsc::ServerParam::instance().parse( msg.c_str(), 999.0 );
    M_default_type = rcsc::PlayerType();
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addPlayerParam( const std::string & msg )
{
    //std::cerr << "ViewHolder::addPlayerParam" << std::endl;
    rcsc::PlayerParam::instance().parse( msg.c_str(), 999.0 );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::addPlayerType( const std::string & msg )
{
    rcsc::PlayerType player_type( msg.c_str(), 999.0 );

    //std::cerr << "ViewHolder::addPlayerType " << player_type.id() << std::endl;
    if ( player_type.id() < 0 )
    {
        std::cerr << "ViewHolder::addPlayerType(). illegal player type id = "
                  << player_type.id()
                  << std::endl;
        return false;
    }

    if ( M_player_types.find( player_type.id() ) != M_player_types.end() )
    {
        M_player_types[player_type.id()] = player_type;
    }
    else
    {
        M_player_types.insert( std::pair< int, rcsc::PlayerType >( player_type.id(), player_type ) );
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::addMonitorViewData( MonitorViewPtr ptr )
{
    if ( M_latest_view_data )
    {
        if ( ! M_monitor_view_cont.empty() )
        {
            // before adding the latest view data,
            // we should remove old BeforeKickOffMode or TimeOver data
            // from data container if exist.

            if ( ptr->playmode().mode() == rcsc::PM_BeforeKickOff
                 || ptr->playmode().mode() == rcsc::PM_TimeOver )
            {
                rcsc::PlayMode back_pmode
                    = M_monitor_view_cont.back()->playmode().mode();
                if ( back_pmode == rcsc::PM_BeforeKickOff
                     || back_pmode == rcsc::PM_TimeOver )
                {
                    // last data must be swapped.
                    M_monitor_view_cont.pop_back();
                }
            }
            else if ( Options::instance().monitorClientMode()
                      && ! Options::instance().timeShiftReplay() )
            {
                M_monitor_view_cont.pop_back();
            }
        }

        M_monitor_view_cont.push_back( M_latest_view_data );
    }

    M_latest_view_data = ptr;
}

/*-------------------------------------------------------------------*/
/*!
  this methos should be called just after finish to open rcg file.
*/
void
ViewHolder::pushBackLatestViewData()
{
    if ( M_latest_view_data )
    {
        M_monitor_view_cont.push_back( M_latest_view_data );
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
void
ViewHolder::addDebugView( const int cycle,
                          char side,
                          const int unum,
                          DebugViewPtr debug_view )
{
    typedef std::map< int, DebugViewCont >::iterator IterType;

    if ( ! debug_view )
    {
        return;
    }

    if ( unum < 1 || 12 < unum )
    {
        std::cerr << "ViewHolder::addDebugView. Error unum over range"
                  << std::endl;
        return;
    }

    std::map< int, DebugViewCont > * view_map = ( side == 'l'
                                                  ? &M_left_debug_view
                                                  : &M_right_debug_view );

    IterType it = view_map->find( cycle );

    // already exist this cycle data
    if ( it != view_map->end() )
    {
        it->second[unum - 1] = debug_view;
        return;
    }

    // insert new cycle (include coach)
    std::pair< IterType, bool > p
        = view_map->insert ( std::make_pair( cycle, DebugViewCont( 12 ) ) );

    if ( ! p.second )
    {
        std::cerr << "ViewHolder::addDebugView. Failed to add DebugView"
                  << std::endl;
        return;
    }

    p.first->second[unum - 1] = debug_view;
}

/*-------------------------------------------------------------------*/
/*!

*/
MonitorViewConstPtr
ViewHolder::getViewData( const std::size_t idx ) const
{
    if ( M_monitor_view_cont.size() <= idx )
    {
        return MonitorViewConstPtr(); // null pointer
    }

    return M_monitor_view_cont[idx];
}


/*-------------------------------------------------------------------*/
/*!

*/
namespace {

struct TimeCmp {
    bool operator()( const MonitorViewPtr & lhs,
                     const int rhs )
      {
          return lhs->cycle() < rhs;
      }
};

}

/*-------------------------------------------------------------------*/
/*!

*/
std::size_t
ViewHolder::getIndexOf( const int cycle ) const
{
    MonitorViewCont::const_iterator it
        = std::lower_bound( M_monitor_view_cont.begin(),
                            M_monitor_view_cont.end(),
                            cycle,
                            TimeCmp() );
    if ( it == M_monitor_view_cont.end() )
    {
        return 0;
    }

    return std::distance( M_monitor_view_cont.begin(), it );

//     MonitorViewCont::size_type len = M_monitor_view_cont.size();
//     MonitorViewCont::size_type half;

//     MonitorViewCont::const_iterator first = M_monitor_view_cont.begin();
//     MonitorViewCont::const_iterator middle;

//     // lower bound algorithm
//     while ( len > 0 )
//     {
//         half = len >> 1;
//         middle = first;
//         middle += half;
//         if ( (*middle)->cycle() < cycle )
//         {
//             first = middle;
//             ++first;
//             len = len - half - 1;
//         }
//         else
//         {
//             len = half;
//         }
//     }

//     if ( first == M_monitor_view_cont.end() )
//     {
//         if ( cycle > 0
//              && ! M_monitor_view_cont.empty() )
//         {
//             return M_monitor_view_cont.size() - 1;
//         }
//         return 0;
//     }

//     return std::distance( M_monitor_view_cont.begin(), first );
}

/*-------------------------------------------------------------------*/
/*!

*/
const
rcsc::PlayerType &
ViewHolder::playerType( const int id ) const
{
    std::map< int, rcsc::PlayerType >::const_iterator it
        = M_player_types.find( id );

    if ( it == M_player_types.end() )
    {
        return M_default_type;
    }

    return it->second;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
ViewHolder::analyzeTeamGraphic( const std::string & msg )
{
    char side = '?';
    int x = -1, y = -1;

    if ( std::sscanf( msg.c_str(),
                      "(team_graphic_%c ( %d %d ",
                      &side, &x, &y ) != 3
         || ( side != 'l' && side != 'r' )
         || x < 0
         || y < 0 )
    {
        std::cerr << "Illegal team_graphic message ["
                  << msg << "]" << std::endl;
        return false;
    }

    if ( side == 'l' )
    {
        //std::cerr << "recv team_graphic_l (" << x << ',' << y << ')'
        //          << std::endl;
        return M_team_graphic_left.parse( msg.c_str() );
    }

    if ( side == 'r' )
    {
        //std::cerr << "recv team_graphic_r (" << x << ',' << y << ')'
        //          << std::endl;
        return M_team_graphic_right.parse( msg.c_str() );
    }

    return false;
}
