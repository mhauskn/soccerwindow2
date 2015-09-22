// -*-c++-*-

/*!
  \file main_data.h
  \brief main data supplier class Header File.
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

#ifndef SOCCERWINDOW2_MAIN_DATA_H
#define SOCCERWINDOW2_MAIN_DATA_H

#include "debug_log_holder.h"
#include "trainer_data.h"
#include "view_holder.h"

class MainData {
private:

    TrainerData M_trainer_data;

    //! view data holder
    ViewHolder M_view_holder;

    //! current view data index in M_view_holder
    std::size_t M_view_index;

    DebugLogHolder M_debug_log_holder;


    // not used
    MainData( const MainData & );
    const MainData & operator=( const MainData & );

public:

    MainData();
    ~MainData();

    void clear();

    bool openRCG( const std::string & file_path );

    bool saveRCG( const std::string & file_path );


    TrainerData & getTrainerData()
      {
          return M_trainer_data;
      }

    const
    TrainerData & trainerData() const
      {
          return M_trainer_data;
      }


    ViewHolder & getViewHolder()
      {
          return M_view_holder;
      }

    const
    ViewHolder & viewHolder() const
      {
          return M_view_holder;
      }

    std::size_t viewIndex() const
      {
          return M_view_index;
      }

    DebugLogHolder & getDebugLogHolder()
      {
          return M_debug_log_holder;
      }

    const
    DebugLogHolder & debugLogHolder() const
      {
          return M_debug_log_holder;
      }

    MonitorViewConstPtr getViewData( const std::size_t idx ) const
      {
          return viewHolder().getViewData( idx );
      }


    MonitorViewConstPtr getCurrentViewData() const
      {
          return viewHolder().getViewData( viewIndex() );
      }

    //! update player selection, focus point, field size, and so on.
    void update( const int width,
                 const int height );

private:

    void selectBallNearestPlayer( MonitorViewConstPtr view );

public:

    bool setViewDataIndexFirst();
    bool setViewDataIndexLast();

    bool setViewDataStepBack();
    bool setViewDataStepForward();

    bool setViewDataIndex( const int index );
    bool setViewDataCycle( const int cycle );
};

#endif
