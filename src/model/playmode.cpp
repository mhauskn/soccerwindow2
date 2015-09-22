// -*-c++-*-

/*!
  \file playmode.cpp
  \brief playmode wrapper class Source File.
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

#include "playmode.h"


/*-------------------------------------------------------------------*/
/*!
  \return playmode string.
*/
const
std::string &
PlayMode::name() const
{
    static std::string s_unknown( "unknown_playmode" );
    static const std::string s_playmode_str[] = PLAYMODE_STRINGS;

    if ( mode() <= rcsc::PM_Null
         || rcsc::PM_MAX <= mode() )
    {
        return s_unknown;
    }

    return s_playmode_str[mode()];
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
PlayMode::isLeftSetPlay() const
{
    switch ( mode() ) {
    case rcsc::PM_KickOff_Left:
    case rcsc::PM_KickIn_Left:
    case rcsc::PM_FreeKick_Left:
    case rcsc::PM_CornerKick_Left:
    case rcsc::PM_GoalKick_Left:
    case rcsc::PM_OffSide_Right:
    case rcsc::PM_Back_Pass_Right:
    case rcsc::PM_Free_Kick_Fault_Right:
    case rcsc::PM_CatchFault_Right:
    case rcsc::PM_IndFreeKick_Left:
    case rcsc::PM_PenaltySetup_Left:
    case rcsc::PM_PenaltyReady_Left:
        return true;
    default:
        break;
    }
    return false;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
PlayMode::isRightSetPlay() const
{
    switch ( mode() ) {
    case rcsc::PM_KickOff_Right:
    case rcsc::PM_KickIn_Right:
    case rcsc::PM_FreeKick_Right:
    case rcsc::PM_CornerKick_Right:
    case rcsc::PM_GoalKick_Right:
    case rcsc::PM_OffSide_Left:
    case rcsc::PM_Back_Pass_Left:
    case rcsc::PM_Free_Kick_Fault_Left:
    case rcsc::PM_CatchFault_Left:
    case rcsc::PM_IndFreeKick_Right:
    case rcsc::PM_PenaltySetup_Right:
    case rcsc::PM_PenaltyReady_Right:
        return true;
    default:
        break;
    }
    return false;
}
