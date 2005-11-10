/***************************************************************************
                          gradienteditor  -  description
                             -------------------
    begin                : Mit Mai 26 2004
    copyright            : (C) 2004 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gradienteditor.h"
#include <qpainter.h>
#include <qpixmap.h>
#include <qcursor.h>
//Added by qt3to4:
#include <QApplication>
#include <QPaintEvent>
#include <Q3PointArray>
#include <QEvent>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "scpainter.h"

extern QPixmap loadIcon(QString nam);

GradientPreview::GradientPreview(QWidget *pa) : QLabel(pa)
{
	setFrameShape( Q3Frame::Panel );
	setFrameShadow( Q3Frame::Sunken );
	setLineWidth( 2 );
	setEraseColor(QColor(255,255,255));
	setMinimumSize(QSize(200, 60));
	setMaximumSize(QSize(3000, 60));
	setMouseTracking(true);
	Mpressed = false;
	outside = true;
	onlyselect = true;
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();

	QColor color;
	color = QColor(255,255,255);
	fill_gradient.addStop( color, 0.0, 0.5, 1.0 );
	color = QColor(0,0,0);
	fill_gradient.addStop( color, 1.0, 0.5, 1.0 );

	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	StopM.clear();
	ActStop = 0;
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		StopM.append(center);
	}
} 

void GradientPreview::paintEvent(QPaintEvent *e)
{
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	StopM.clear();
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		StopM.append(center);
	}
	QPixmap pixm(width()-20, 37);
	ScPainter *p = new ScPainter(&pixm, width()-20, 37);
	p->setPen(Qt::black);
	p->setLineWidth(1);
	p->setFillMode(2);
	p->fill_gradient = fill_gradient;
	p->setGradient(VGradient::linear, QPointF(0,20), QPointF(width()-20,20));
	p->drawRect(0, 0, width()-20, 37);
	p->end();
	delete p;
	QPainter pw;
	pw.begin(this);
	pw.drawPixmap(10, 5, pixm);
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		pw.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		if (StopM[QMAX(ActStop,0)] == center)
			pw.setBrush(Qt::red);
		else
			pw.setBrush(Qt::blue);
		Q3PointArray cr;
		cr.setPoints(3, qRound(center), 43, qRound(center-4), 56, qRound(center+4), 56);
		pw.drawPolygon(cr);
	}
	pw.end();
	QLabel::paintEvent(e);
}

void GradientPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	Mpressed = true;
	ActStop = -1;
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
	{
		fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
		if (fpo.contains(m->pos()))
		{
			ActStop = yg;
			emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
			emit currTrans(cstops.at(ActStop)->opacity);
			emit currStep(cstops.at(ActStop)->rampPoint);
			repaint();
			onlyselect = true;
			return;
		}
	}
}

void GradientPreview::mouseReleaseEvent(QMouseEvent *m)
{
	QRect fpo;
	if ((Mpressed) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1) && (outside))
	{
		onlyselect = false;
		fill_gradient.removeStop(ActStop);
		ActStop = 0;
		repaint();
		Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
		emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
		emit currTrans(cstops.at(ActStop)->opacity);
		emit currStep(cstops.at(ActStop)->rampPoint);
	}
	if ((m->y() < height()) && (m->y() > 43) && (m->x() > 0) && (m->x() < width()) && (ActStop == -1))
	{
		Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
		double newStop = static_cast<double>((m->x() - 10)) / (static_cast<double>(width())-20);
		fill_gradient.addStop(cstops.at(0)->color, newStop, 0.5, 1.0, cstops.at(0)->name, cstops.at(0)->shade);
		repaint();
		onlyselect = false;
		cstops = fill_gradient.colorStops();
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				ActStop = yg;
				emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
				emit currTrans(cstops.at(ActStop)->opacity);
				emit currStep(cstops.at(ActStop)->rampPoint);
				repaint();
				break;
			}
		}
	}
	Mpressed = false;
	if (!onlyselect)
		emit gradientChanged();
}

void GradientPreview::mouseMoveEvent(QMouseEvent *m)
{
	QRect fpo;
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
	if ((!Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 9) && (m->x() < width()-9))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("AddPoint.png"), 4, 3), true);
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
				return;
			}
		}
	}
	if ((Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 9) && (m->x() < width()-9) && (ActStop != -1))
	{
		qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
		double newStop = static_cast<double>((m->x() - 10)) / (static_cast<double>(width())-20);
		if (ActStop > 1)
		{
			if (StopM[ActStop-1]+2 >= m->x())
				return;
		}
		if (ActStop < static_cast<int>(StopM.count()-2))
		{
			if (StopM[ActStop+1]-2 < m->x())
				return;
		}
		StopM[ActStop] = m->x();
		Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
		cstops.at(ActStop)->rampPoint = newStop;
		emit currStep(cstops.at(ActStop)->rampPoint);
		cstops.sort();
		onlyselect = false;
		repaint();
	}
	if ((Mpressed) && (outside) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1))
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
}

void GradientPreview::leaveEvent(QEvent*)
{
	if ((Mpressed) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1))
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
	else
		qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
	outside = true;
}

void GradientPreview::enterEvent(QEvent*)
{
	outside = false;
}

void GradientPreview::updateDisplay()
{
	repaint();
	ActStop = 0;
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
	emit currTrans(cstops.at(ActStop)->opacity);
	emit currStep(cstops.at(ActStop)->rampPoint);
}

void GradientPreview::setActColor(QColor c, QString n, int s)
{
	if (ActStop == -1)
		return;
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->color = c;
	cstops.at(ActStop)->name = n;
	cstops.at(ActStop)->shade = s;
	repaint();
}

void GradientPreview::setActTrans(double t)
{
	if (ActStop == -1)
		return;
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->opacity = t;
	repaint();
}

void GradientPreview::setActStep(double t)
{
	if (ActStop == -1)
		return;
	Q3PtrVector<VColorStop> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->rampPoint = t;
	repaint();
}

GradientEditor::GradientEditor(QWidget *pa) : QLabel(pa)
{
	Form1Layout = new QVBoxLayout( this, 0, 0, "Form1Layout");
	Preview = new GradientPreview(this);
	Form1Layout->addWidget(Preview);
	Layout1 = new QHBoxLayout(0,0,0,"Layout1");
	Layout1->setSpacing( 4 );
	Layout1->setMargin( 0 );
	Desc = new QLabel( this, "Desc" );
	Desc->setText( tr( "Position:" ) );
	Layout1->addWidget( Desc );
	Position = new QSpinBox( this, "shspin" );
	Position->setMinValue(0);
	Position->setMaxValue(100);
	Position->setLineStep(1);
	Position->setSuffix( tr(" %"));
	Position->setValue(0);
	Layout1->addWidget(Position);
	Form1Layout->addLayout(Layout1);
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
	connect(Preview, SIGNAL(currStep(double)), this, SLOT(setPos(double)));
	QToolTip::add( Preview, QRect(10,43, Preview->width()-20, 13), tr( "Here you can add, change or remove Color-Stops." ) );
	resize(minimumSizeHint());
}

void GradientEditor::setPos(double p)
{
	disconnect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
	Position->setValue(qRound(p * 100));
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
}

void GradientEditor::changePos(int v)
{
	Preview->setActStep(static_cast<double>(v) / 100.0);
	emit gradientChanged();
}
