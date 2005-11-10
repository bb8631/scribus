/* This file is part of the KDE project
   Copyright (C) 2002, The Karbon Developers
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __VGRADIENT_H__
#define __VGRADIENT_H__

#include <q3ptrlist.h>
#include <q3ptrvector.h>
#include "scribusapi.h"
#include <qcolor.h>
#include <qmatrix.h>

class SCRIBUS_API VColorStop
{
public:
	VColorStop( double r, double m, QColor c, double o, QString n, int s )
	{
		rampPoint = r;
		midPoint = m; 
		color = c;
		opacity = o; 
		name = n;
		shade = s;
	};
	
	VColorStop( const VColorStop& colorStop )
	{
		rampPoint = colorStop.rampPoint;
		midPoint = colorStop.midPoint;
		color = colorStop.color;
		opacity = colorStop.opacity;
		name = colorStop.name;
		shade = colorStop.shade;
	};

	QColor color;

	// relative position of color point (0.0-1.0):
	double rampPoint;

	// relative position of midpoint (0.0-1.0)
	// between two ramp points. ignored for last VColorStop.
	double midPoint;
	double opacity;
	int shade;
	QString name;
	friend inline bool operator== ( VColorStop& s1, VColorStop& s2 )
	{ return s1.rampPoint == s2.rampPoint; };
}
; // VColorStop

class SCRIBUS_API VGradient
{
	// friend class VGradientWidget;

public:
	enum VGradientType
	{
	    linear = 0,
	    radial = 1,
	    conic  = 2
	};

	enum VGradientRepeatMethod
	{
	    none    = 0,
	    reflect = 1,
	    repeat  = 2
	};

class SCRIBUS_API VColorStopList : public Q3PtrList<VColorStop>
	{
	protected:
		virtual int compareItems( Q3PtrCollection::Item item1, Q3PtrCollection::Item item2 );
	}
	; // VColorStopList

	VGradient( VGradientType type = linear );
	VGradient( const VGradient& gradient );

	VGradient& operator=(const VGradient& gradient);

	VGradientType type() const { return m_type; }
	void setType( VGradientType type ) { m_type = type; }

	VGradientRepeatMethod repeatMethod() const { return m_repeatMethod; }
	void setRepeatMethod( VGradientRepeatMethod repeatMethod ) { m_repeatMethod = repeatMethod; }

	const Q3PtrVector<VColorStop> colorStops() const;
	void addStop( const VColorStop& colorStop );
	void addStop( const QColor &color, double rampPoint, double midPoint, double opa, QString name = "", int shade = 100 );
	void removeStop( const VColorStop& colorStop );
	void removeStop( uint n );
	void clearStops();
	uint Stops() { return m_colorStops.count(); }

	QPointF origin() const { return m_origin; }
	void setOrigin( const QPointF &origin ) { m_origin = origin; }

	QPointF focalPoint() const { return m_focalPoint; }
	void setFocalPoint( const QPointF &focalPoint ) { m_focalPoint = focalPoint; }

	QPointF vector() const { return m_vector; }
	void setVector( const QPointF &vector ) { m_vector = vector; }

	void transform( const QMatrix& m );

protected:
	VColorStopList        m_colorStops;

private:
	VGradientType         m_type			: 2;
	VGradientRepeatMethod m_repeatMethod	: 2;

	// coordinates:
	QPointF m_origin;
	QPointF m_focalPoint;
	QPointF m_vector;
}
; // VGradient

#endif /* __VGRADIENT_H__ */
