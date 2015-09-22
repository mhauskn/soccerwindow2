// -*-c++-*-

/*!
  \file team.cpp
  \brief team data class Source File.
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

#include "team.h"

#include <rcsc/rcg/util.h>

#include <cstring>

/*-------------------------------------------------------------------*/
/*!

*/
Team::Team( const rcsc::rcg::team_t & team )
{
    assign( team );
}

/*-------------------------------------------------------------------*/
/*!

*/
Team::Team( const rcsc::rcg::TeamT & team )
    : M_data( team )
{

}

/*-------------------------------------------------------------------*/
/*!

*/
void
Team::assign( const rcsc::rcg::team_t & team )
{
    char buf[18];
    std::memset( buf, '\0', 18 );
    std::strncpy( buf, team.name, 16 );

    M_data.name_ = buf;
    M_data.score_ = ntohs( team.score );
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Team::assign( const rcsc::rcg::TeamT & team )
{
    M_data = team;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
Team::convertTo( rcsc::rcg::team_t & team ) const
{
    std::memset( team.name, 0, sizeof( team.name ) );
    std::strncpy( team.name,
                  M_data.name_.c_str(),
                  std::min( sizeof( team.name ), M_data.name_.length() ) );
    team.score = htons( M_data.score_ );
}
