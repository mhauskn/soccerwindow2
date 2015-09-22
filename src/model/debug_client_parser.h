// -*-c++-*-

/*!
  \file debug_client_parser.h
  \brief debug client paket parser class Header File.
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

#ifndef SOCCERWINDOW2_DEBUG_CLIENT_PARSER_H
#define SOCCERWINDOW2_DEBUG_CLIENT_PARSER_H

#include "debug_view_data.h"

class ViewHolder;

/*!
  \class DebugClientParser
  \brief the class that is used to parse debug client packat.
*/
class DebugClientParser {
private:
    char M_side; //!< sender's side
    int M_unum; //!< sender's unum
    int M_cycle; //!< cycle of sent data

public:
    DebugClientParser()
        : M_side( 'n' )
        , M_unum( 0 )
        , M_cycle( -1 )
      { }

    bool parse( const char * msg,
                ViewHolder & holder );

private:
    bool parseV2( const char * msg,
                  DebugViewPtr & debug_view );


    int parseTime( const char * tok,
                   DebugViewPtr & debug_view );

    int parseBall( const char * tok,
                   DebugViewPtr & debug_view );
    int parseSelf( const char * tok,
                   DebugViewPtr & debug_view );
    int parsePlayer( const char * tok,
                     DebugViewPtr & debug_view );
    int parseLine( const char * tok,
                   DebugViewPtr & debug_view );
    int parseTriangle( const char * tok,
                       DebugViewPtr & debug_view );
    int parseRectangle( const char * tok,
                        DebugViewPtr & debug_view );
    int parseCircle( const char * tok,
                     DebugViewPtr & debug_view );
    int parseTargetTeammate( const char * tok,
                             DebugViewPtr & debug_view );
    int parseTargetPoint( const char * tok,
                          DebugViewPtr & debug_view );
    int parseMessage( const char * tok,
                      DebugViewPtr & debug_view );
    int parseSayMessage( const char * tok,
                         DebugViewPtr & debug_view );
    int parseHearMessage( const char * tok,
                          DebugViewPtr & debug_view );

};

#endif
