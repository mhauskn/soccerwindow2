// -*-c++-*-

/*!
  \file team.h
  \brief team data wrapper class Header File.
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

#ifndef SOCCERWINDOW2_MODEL_TEAM_H
#define SOCCERWINDOW2_MODEL_TEAM_H

#include <rcsc/rcg/types.h>
#include <string>

/*
   \class Team
   \brief team info wrapper class.
 */
class Team {
private:
    rcsc::rcg::TeamT M_data;
public:

    Team()
      { }

    explicit
    Team( const rcsc::rcg::team_t & team );

    explicit
    Team( const rcsc::rcg::TeamT & team );

    void assign( const rcsc::rcg::team_t & team );

    void assign( const rcsc::rcg::TeamT & team );

    void convertTo( rcsc::rcg::team_t & team ) const;

    // accessor

    const
    rcsc::rcg::TeamT & data() const
      {
          return M_data;
      }

    const
    std::string & name() const
      {
          return M_data.name_;
      }

    int score() const
      {
          return M_data.score_;
      }

    int penScore() const
      {
          return M_data.pen_score_;
      }

    int penMiss() const
      {
          return M_data.pen_miss_;
      }

    int penTrial() const
      {
          return M_data.pen_score_ + M_data.pen_miss_;
      }

};

#endif
