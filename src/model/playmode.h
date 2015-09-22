// -*-c++-*-

/*!
  \file playmode.h
  \brief playmode wrapper class Header File.
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

#ifndef SOCCERWINDOW2_MODEL_PLAYMODE_H
#define SOCCERWINDOW2_MODEL_PLAYMODE_H

#include <rcsc/types.h>
#include <string>

/*
   \class PlayMode
   \brief playmode wrapper class.
 */
class PlayMode {
private:
    rcsc::PlayMode M_mode;
public:

    PlayMode()
        : M_mode( rcsc::PM_Null )
      { }

    PlayMode( const char pm )
        : M_mode( static_cast< rcsc::PlayMode >( pm ) )
      { }

    PlayMode( rcsc::PlayMode pm )
        : M_mode( pm )
      { }

    rcsc::PlayMode mode() const
      {
          return M_mode;
      }

    const
    std::string & name() const;

    bool isLeftSetPlay() const;

    bool isRightSetPlay() const;
};

#endif
