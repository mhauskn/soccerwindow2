// -*-c++-*-

/*!
  \file debug_log_holder.cpp
  \brief debug data repository Source File.
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

#include "debug_log_holder.h"

#include "debug_log_data.h"

#include <sstream>
#include <iostream>
#include <cstdio>

namespace {
const int MAX_SPAN = 1;
}

const std::string DebugLogHolder::DEFAULT_EXTENSION = ".log";

/*-------------------------------------------------------------------*/
/*!

*/
DebugLogHolder::~DebugLogHolder()
{
    clear();
    //std::cerr << "delete DebugLogHolder" << std::endl;
}


/*-------------------------------------------------------------------*/
/*!

*/
void
DebugLogHolder::clear()
{
    for ( int i = 1; i <= 12; ++i )
    {
        M_data_entity[i-1].clear();
    }
}

/*-------------------------------------------------------------------*/
/*!
  \return if available file is found more then one, returun true.
*/
bool
DebugLogHolder::setDir( const std::string & team_name,
                        const rcsc::SideID side,
                        const int unum,
                        const std::string & dirpath )
{
    if ( team_name.empty() )
    {
        std::cerr << "***ERROR*** DebugLogHolder::setDir()"
                  << " No team name!" << std::endl;
        return false;
    }

    if ( unum < 1 || 12 < unum )
    {
        std::cerr << "***ERROR*** DebugLogHolder::setDir()"
                  << " Invalid unum! " << unum
                  << std::endl;
        return false;
    }


    std::string base_path = dirpath;
    if ( ! base_path.empty()
         && base_path[base_path.size() - 1] != '/' )
    {
        base_path += '/';
    }

    //for ( int i = 1; i <= 11; ++i )
    {
        DataEntity & de = M_data_entity[unum - 1];
        de.clear();

        de.side_ = side;

        // open file stream
        std::string filepath = base_path;
        {
            std::ostringstream ostr;
            if ( unum == 12 )
            {
                ostr << team_name << "-coach.log";
            }
            else
            {
                ostr << team_name << '-' << unum << ".log";
            }

            filepath += ostr.str();
        }

        std::cerr << "open debug file [" << filepath << "]" << std::endl;
        de.fin_.open( filepath.c_str() );

        if ( ! de.fin_.is_open()
             || ! de.fin_.good() )
        {
            std::cerr << "***ERROR*** DebugLogHolder::setDir()"
                      << " Failed to open [" << filepath << "]"
                      << std::endl;
            de.clear();
            return false;
        }

        // record stream positions where cycle number is changed.

        std::streampos pos = 0;
        std::string buf;
        int prev_cycle = -1;
        int cycle;

        int n_line = 0;
        std::ifstream & fin = de.fin_;
        while ( std::getline( fin, buf ) )
        {
            ++n_line;
            // first element should be cycle value.
            if ( std::sscanf( buf.c_str(), " %d ", &cycle ) == 1 )
            {
                if ( cycle > prev_cycle )
                {
                    de.cycle_map_.insert( std::make_pair( cycle, pos ) );
                    prev_cycle = cycle;
                }

                if ( cycle < prev_cycle )
                {
                    std::cerr << "***ERROR*** DebugLogHolder::setDir()"
                              << " Invalid cycle! path= "<< filepath
                              << "  line= " << n_line
                              << std::endl;
                }
            }
            pos = fin.tellg();
        }

        // reset get pointer
        de.fin_.clear();
        de.fin_.seekg( 0 );

        std::cerr << unum << " : read " << n_line << " lines"
                  << " cycle size = " << de.cycle_map_.size()
                  << std::endl;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::getCycleData( const int unum,
                              const int cycle,
                              boost::shared_ptr< DebugLogData > data_ptr )
{
    if ( unum < 1 || 12 < unum )
    {
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    // get the start stream position of 'cycle'
    std::map< int, std::streampos >::iterator it = de.cycle_map_.find( cycle );

    if ( it == de.cycle_map_.end() )
    {
        // no data!
        return false;
    }

    ///////////////////////////////////////////////////
    // seek & analyze data

    // seek stream position
    std::ifstream & fin = de.fin_;
    fin.clear();
    fin.seekg( it->second );

    // read loop
    char buf[8192];
    int n_line = 0;
    while ( fin.getline( buf, 8192 ) )
    {
        if ( ++n_line > 8192 * 8 )
        {
            std::cerr << "***ERROR*** DebugLogHolder::getCycleData()"
                      << " Too many lines! cycle ="
                      << cycle
                      << std::endl;
            break;
        }

        int read_cycle = 0;
        int n_read = 0;
        if ( std::sscanf( buf, " %d %n", &read_cycle, &n_read ) != 1
             || n_read == 0 )
        {
            // invalide cycle line.
            continue;
        }

        // reach new cycle
        if ( read_cycle > cycle )
        {
            break;
        }

        if ( ! data_ptr->parse( buf + n_read ) )
        {
            std::cerr << "***ERROR*** DebugLogHolder::getCycleData()"
                      << " Parse Error! unum = " << unum
                      << "  cycle = " << cycle
                      << "  line = " << n_line
                      << std::endl;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::seekData( const int unum,
                          const int cycle )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << "DebugLogHolder::seekData. Invalid number"
                  << std::endl;
        return false;
    }

    /*
    if ( s_last_seek_number == unum
         && M_cycle == cycle )
    {
        return false;
    }
    */

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        return false;
    }

    ///////////////////////////////////////////////////
    // get recent span stream position

    bool found = false;
    std::streampos first_pos = 0;
    int first_cycle = std::max( 1, cycle - MAX_SPAN + 1 );
    for ( ; first_cycle <= cycle; ++first_cycle )
    {
        std::map< int, std::streampos >::iterator it = de.cycle_map_.find( first_cycle );
        if ( it != de.cycle_map_.end() )
        {
            found = true;
            first_pos = it->second;
            break;
        }
    }

    if ( ! found  )
    {
        std::cerr << "***ERROR*** DebugLogHolder::seekData()"
                  << " could not find stream position."
                  << " unum = " << unum
                  << " cycle = " << cycle
                  << std::endl;
        return false;
    }

    ///////////////////////////////////////////////////
    // seek & analyze data

    // reset old data
    de.data_list_.clear();

    const int last_cycle = cycle + MAX_SPAN;
    for ( int c = first_cycle; c < last_cycle; ++c )
    {
        boost::shared_ptr< DebugLogData > data( new DebugLogData( c ) );
        if ( getCycleData( unum, c, data ) )
        {
            de.data_list_.push_back( data );
        }
    }

    M_cycle = cycle;
    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::incDataCycle( const int unum )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << "***ERROR*** DebugLogHolder::incDataCycle()"
                  << " Invalid unum " << unum
                  << std::endl;
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        //std::cerr << "***ERROR*** DebugLogHolder::incDataCycle()"
        //          << " File is not opened for unum = " << unum
        //          << std::endl;
        return false;
    }

    if ( de.data_list_.empty() )
    {
        std::cerr << "***ERROR*** DebugLogHolder::incDataCycle()"
                  << " No current data! unum = " << unum
                  << std::endl;
        return false;
    }

    for ( DataList::iterator it = de.data_list_.begin(),
              end = de.data_list_.end();
          it != end;
          ++it )
    {
        if ( (*it)->cycle() == M_cycle )
        {
            // The data cycle equals to the current focused cycle
            ++it;
            if ( it != end )
            {
                M_cycle = (*it)->cycle();
                return true;
            }
            break;
        }
    }

    return seekData( unum, M_cycle + 1 );
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
DebugLogHolder::decDataCycle( const int unum )
{
    if ( unum < 1 || 12 < unum )
    {
        std::cerr << "***ERROR*** DebugLogHolder::incDataCycle()"
                  << " Invalid unum " << unum
                  << std::endl;
        return false;
    }

    DataEntity & de = M_data_entity[unum - 1];

    if ( ! de.fin_.is_open() )
    {
        //std::cerr << "***ERROR*** DebugLogHolder::decDataCycle()"
        //          << " File is not opened for unum = " << unum
        //          << std::endl;
        return false;
    }

    if ( de.data_list_.empty() )
    {
        std::cerr << "***ERROR*** DebugLogHolder::decDataCycle()"
                  << " No current data! unum = " << unum
                  << std::endl;
        return false;
    }

    for ( DataList::reverse_iterator it = de.data_list_.rbegin(),
              rend = de.data_list_.rend();
          it != rend;
          ++it )
    {
        if ( (*it)->cycle() == M_cycle )
        {
            ++it;
            if ( it != rend )
            {
                M_cycle = (*it)->cycle();
                return true;
            }
            break;
        }
    }

    return seekData( unum, M_cycle - 1 );
}
