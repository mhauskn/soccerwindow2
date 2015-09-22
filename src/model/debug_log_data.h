// -*-c++-*-

/*!
  \file debug_log_data.h
  \brief debug log data Header File.
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

#ifndef SOCCERWINDOW2_DEBUG_LOG_DATA_H
#define SOCCERWINDOW2_DEBUG_LOG_DATA_H

#include <boost/cstdint.hpp>

#include <string>
#include <list>
#include <map>

//! one cycle debug data snapshot
class DebugLogData {
public:

    static const boost::int32_t LEVEL_00 = 0x00000000;
    static const boost::int32_t LEVEL_01 = 0x00000001;
    static const boost::int32_t LEVEL_02 = 0x00000002;
    static const boost::int32_t LEVEL_03 = 0x00000004;
    static const boost::int32_t LEVEL_04 = 0x00000008;
    static const boost::int32_t LEVEL_05 = 0x00000010;
    static const boost::int32_t LEVEL_06 = 0x00000020;
    static const boost::int32_t LEVEL_07 = 0x00000040;
    static const boost::int32_t LEVEL_08 = 0x00000080;
    static const boost::int32_t LEVEL_09 = 0x00000100;
    static const boost::int32_t LEVEL_10 = 0x00000200;
    static const boost::int32_t LEVEL_11 = 0x00000400;
    static const boost::int32_t LEVEL_12 = 0x00000800;
    static const boost::int32_t LEVEL_13 = 0x00001000;
    static const boost::int32_t LEVEL_14 = 0x00002000;
    static const boost::int32_t LEVEL_15 = 0x00004000;
    static const boost::int32_t LEVEL_16 = 0x00008000;
    static const boost::int32_t LEVEL_17 = 0x00010000;
    static const boost::int32_t LEVEL_18 = 0x00020000;
    static const boost::int32_t LEVEL_19 = 0x00040000;
    static const boost::int32_t LEVEL_20 = 0x00080000;
    static const boost::int32_t LEVEL_21 = 0x00100000;
    static const boost::int32_t LEVEL_22 = 0x00200000;
    static const boost::int32_t LEVEL_23 = 0x00400000;
    static const boost::int32_t LEVEL_24 = 0x00800000;
    static const boost::int32_t LEVEL_25 = 0x01000000;
    static const boost::int32_t LEVEL_26 = 0x02000000;
    static const boost::int32_t LEVEL_27 = 0x04000000;
    static const boost::int32_t LEVEL_28 = 0x08000000;
    static const boost::int32_t LEVEL_29 = 0x10000000;
    static const boost::int32_t LEVEL_30 = 0x20000000;
    static const boost::int32_t LEVEL_31 = 0x40000000;
    static const boost::int32_t LEVEL_32 = 0x80000000;
    static const boost::int32_t LEVEL_ANY = 0xffffffff;

    struct TextT {
        boost::int32_t level_;
        std::string msg_;
        TextT()
            : level_( 0 )
          { }
        TextT( const int level )
            : level_( level )
          { }
    };
    struct PointT {
        boost::int32_t level_;
        double x_;
        double y_;
        std::string color_;
        PointT()
            : level_( 0 )
            , x_( 0.0 )
            , y_( 0.0 )
          { }
    };
    struct LineT {
        boost::int32_t level_;
        double x1_;
        double y1_;
        double x2_;
        double y2_;
        std::string color_;
        LineT()
            : level_( 0 )
            , x1_( 0.0 )
            , y1_( 0.0 )
            , x2_( 0.0 )
            , y2_( 0.0 )
          { }
    };
    struct ArcT {
        boost::int32_t level_;
        double x_;
        double y_;
        double r_;
        double start_angle_;
        double span_angle_;
        std::string color_;
        ArcT()
            : level_( 0 )
            , x_( 0.0 )
            , y_( 0.0 )
            , r_( 0.0 )
            , start_angle_( 0.0 )
            , span_angle_( 0.0 )
          { }
    };
    struct CircleT {
        boost::int32_t level_;
        double x_;
        double y_;
        double r_;
        std::string color_;
        CircleT()
            : level_( 0 )
            , x_( 0.0 )
            , y_( 0.0 )
            , r_( 0.0 )
          { }
    };
    struct TriangleT {
        boost::int32_t level_;
        double x1_;
        double y1_;
        double x2_;
        double y2_;
        double x3_;
        double y3_;
        std::string color_;
        TriangleT()
            : level_( 0 )
            , x1_( 0.0 )
            , y1_( 0.0 )
            , x2_( 0.0 )
            , y2_( 0.0 )
            , x3_( 0.0 )
            , y3_( 0.0 )
          { }
    };
    struct RectT {
        boost::int32_t level_;
        double left_;
        double top_;
        double width_;
        double height_;
        std::string color_;
        RectT()
            : level_( 0 )
            , left_( 0.0 )
            , top_( 0.0 )
            , width_( 0.0 )
            , height_( 0.0 )
          { }
    };
   struct SectorT {
        boost::int32_t level_;
        double x_;
        double y_;
        double min_r_;
        double max_r_;
        double start_angle_;
        double span_angle_;
        std::string color_;
        SectorT()
            : level_( 0 )
            , x_( 0.0 )
            , y_( 0.0 )
            , min_r_( 0.0 )
            , max_r_( 0.0 )
            , start_angle_( 0.0 )
            , span_angle_( 0.0 )
          { }
    };
    struct MessageT {
        boost::int32_t level_;
        double x_;
        double y_;
        std::string message_;
        std::string color_;
        MessageT()
            : level_( 0 )
            , x_( 0.0 )
            , y_( 0.0 )
          { }
    };

    /**********************************************************
    Log Message Line Format
    Line := <Time> <Level> <Type> <Content>
    Time := integer value
    Level := integer value
    Type := {M|p|l|c|C|t|T|r|R|m}
        M : log message text
        p : point
        l: line
        a: arc
        c: circle
        C: filled circle
        t: triangle
        T: filled triangle
        r: rectangle
        R: filled rectangle
        s: sector
        S: filled sector
        m: message;
    Text := <Str>
    Point := <x:Real> <y:Real>[ <Color>]
    Line := <x1:Real> <y1:Real> <x2:Real> <y2:Real>[ <Color>]
    Arc := <x:Real> <y:Real> <r:Real> <angle:Real> <span:Real>[ <Color>]
    Circle := <x:Real> <y:Real> <r:Real>[ <Color>]
    Triangle := <x1:Real> <y1:Real> <x2:Real> <y2:Real> <x3:Real> <y3:Real>[ <Color>]
    Rectangle := <leftX:Real> <topY:Real> <width:Real> <height:Real>[ <Color>]
    Sector := <x:Real> <y:Real> <minRadius:Real> <maxRadius:Real> <angle:Real> <span:Real>[ <Color>]
    Message := <x:Real> <y:Real>[ (c <Color>)] <Str>
    **********************************************************/

    // pair.first is log level
    typedef std::list< TextT > TextCont;

    typedef std::list< PointT > PointCont;
    typedef std::list< LineT > LineCont;
    typedef std::list< ArcT > ArcCont;
    typedef std::list< CircleT > CircleCont;
    typedef std::list< TriangleT > TriangleCont;
    typedef std::list< RectT > RectCont;
    typedef std::list< SectorT > SectorCont;
    typedef std::list< MessageT > MessageCont;
private:
    int M_cycle;

    //! normal message append to debug message box.
    TextCont M_text_cont;

    PointCont M_point_cont;
    LineCont M_line_cont;
    ArcCont M_arc_cont;
    CircleCont M_circle_cont;
    CircleCont M_filled_circle_cont;
    TriangleCont M_triangle_cont;
    TriangleCont M_filled_triangle_cont;
    RectCont M_rect_cont;
    RectCont M_filled_rect_cont;
    SectorCont M_sector_cont;
    SectorCont M_filled_sector_cont;
    MessageCont M_message_cont;

public:
    explicit
    DebugLogData( const int cycle )
        : M_cycle( cycle )
      { }

    void setCycle( const int cycle )
      {
          M_cycle = cycle;
      }

    int cycle() const
      {
          return M_cycle;
      }

    bool parse( const char * buf );

    const
    TextCont & textCont() const
      {
          return M_text_cont;
      }
    const
    PointCont & pointCont() const
      {
          return M_point_cont;
      }
    const
    LineCont & lineCont() const
      {
          return M_line_cont;
      }
    const
    ArcCont & arcCont() const
      {
          return M_arc_cont;
      }
    const
    CircleCont & circleCont() const
      {
          return M_circle_cont;
      }
    const
    CircleCont & filledCircleCont() const
      {
          return M_filled_circle_cont;
      }
    const
    TriangleCont & triangleCont() const
      {
          return M_triangle_cont;
      }
    const
    TriangleCont & filledTriangleCont() const
      {
          return M_filled_triangle_cont;
      }
    const
    RectCont & rectCont() const
      {
          return M_rect_cont;
      }
    const
    RectCont & filledRectCont() const
      {
          return M_filled_rect_cont;
      }
    const
    SectorCont & sectorCont() const
      {
          return M_sector_cont;
      }
    const
    SectorCont & filledSectorCont() const
      {
          return M_filled_sector_cont;
      }
    const
    MessageCont & messageCont() const
      {
          return M_message_cont;
      }

private:

    int parseColor( const char * buf );

    bool addPoint( const boost::int32_t level,
                   const char * buf );
    bool addLine( const boost::int32_t level,
                  const char * buf );
    bool addArc( const boost::int32_t level,
                 const char * buf );
    bool addCircle( const boost::int32_t level,
                    const char * buf,
                    const bool fill );
    bool addTriangle( const boost::int32_t level,
                      const char * buf,
                      const bool fill );
    bool addRect( const boost::int32_t level,
                  const char * buf,
                  const bool fill );
    bool addSector( const boost::int32_t level,
                    const char * buf,
                    const bool fill );
    bool addMessage( const boost::int32_t level,
                     const char * buf );
};

#endif
