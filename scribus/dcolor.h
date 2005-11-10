//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include "sccolor.h"

/****************************************************************************
** Form interface generated from reading ui file 'Cdel.ui'
**
** Created: Tue Apr 24 22:08:26 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DELCOLOR_H
#define DELCOLOR_H

class QLayout;
class QLabel;
class QPushButton;
class QComboBox;

#include "scribusapi.h"

class SCRIBUS_API DelColor : public QDialog
{ 
    Q_OBJECT

public:
    DelColor(QWidget* parent, ColorList colorList, QString colorName, bool haveDoc );
    ~DelColor() {};
	const QString getReplacementColor();

private:
    QLabel* deleteLabel;
    QLabel* colorToDelLabel;
    QLabel* replaceLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QComboBox* replacementColData;
    QVBoxLayout* dialogLayout;
    QGridLayout* delColorLayout;
    QHBoxLayout* okCancelLayout;

    QString replacementColor;

private slots:
    virtual void ReplaceColor(int);
};

#endif // DELCOLOR_H
