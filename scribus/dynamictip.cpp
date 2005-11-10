/***************************************************************************
                          dynamictip.cpp  -  description
                             -------------------
    begin                : Wed Aug 31 2005
    copyright            : (C) 2005 by Franz Schmid
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

#include "dynamictip.h"
#include "seiten.h"

DynamicTip::DynamicTip( Q3ListBox* parent, ColorList* pale )
    : QWidget(parent)
{
	colorList = pale;
	listB = parent;
	kind = ColorListBox;
}

DynamicTip::DynamicTip( Q3Table* parent ) : QWidget(parent)
{
	table = parent;
	kind = Table;
}

void DynamicTip::maybeTip( const QPoint &pos )
{
	if (kind == ColorListBox)
	{
		Q3ListBoxItem* it = listB->itemAt(pos);
		if (it != 0)
		{
			if (!colorList->contains(it->text()))
				return;
			QString tipText = "";
			ScColor col = (*colorList)[it->text()];
			if (col.getColorModel() == colorModelCMYK)
			{
				int c, m, y, k;
				col.getCMYK(&c, &m, &y, &k);
				tipText = QString("C:%1% M:%2% Y:%3% K:%4%").arg(qRound(c / 2.55)).arg(qRound(m / 2.55)).arg(qRound(y / 2.55)).arg(qRound(k / 2.55));
			}
			else
			{
				int r, g, b;
				col.getRawRGBColor(&r, &g, &b);
				tipText = QString("R:%1 G:%2 B:%3").arg(r).arg(g).arg(b);
			}
			QToolTip::showText(listB->itemRect(it).topLeft(), tipText);
		}
	}
	else
	{
		QPoint cp = table->viewportToContents( pos );
		int row = table->rowAt( cp.y() );
		int col = table->columnAt( cp.x() );
		Q3TableItem* ite = table->item(row, col);
		if (ite == 0)
			return;
		QRect cr = table->cellGeometry( row, col );
		cr.moveTopLeft( table->contentsToViewport( cr.topLeft() ) );

		SeItem* it = (SeItem*)ite;
		QString tipString = it->getPageName();
		qWarning("XXX tooltips can't be inherited");
		QToolTip::showText( cr.topLeft(), tipString );
	}
}
