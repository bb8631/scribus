#include "editformats.h"
#include "edit1format.h"
#include <qmessagebox.h>
#include <q3header.h>
//Added by qt3to4:
#include <QPixmap>
#include <QHBoxLayout>
#include <Q3ValueList>
#include <QVBoxLayout>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "customfdialog.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scribusXml.h"
#include "page.h"
#include "sccombobox.h"

extern QPixmap loadIcon(QString nam);

DelStyle::DelStyle(QWidget* parent, Q3ValueList<ParagraphStyle> sty, QString styleName)
		: QDialog( parent, "dd", true, 0 )
{
	setName( "DelColor" );
	setCaption( tr( "Delete Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5 );
	delStyleLayout = new QGridLayout;
	delStyleLayout->setSpacing( 5 );
	delStyleLayout->setMargin( 5 );
	deleteLabel = new QLabel( tr( "Delete Style:" ), this, "deleteLabel" );
	delStyleLayout->addWidget( deleteLabel, 0, 0 );
	styleToDelLabel = new QLabel( styleName, this, "colorToDelLabel" );
	delStyleLayout->addWidget( styleToDelLabel, 0, 1 );
	replaceLabel = new QLabel( tr( "Replace With:" ), this, "replaceLabel" );
	delStyleLayout->addWidget( replaceLabel, 1, 0 );
	replacementStyleData = new ScComboBox(false, this);
	for (int x = 5; x < sty.count(); ++x)
	{
		if (sty[x].Vname != styleName)
			replacementStyleData->insertItem(sty[x].Vname);
	}
	delStyleLayout->addWidget( replacementStyleData, 1, 1 );
	replacementStyle = replacementStyleData->text(0);
	dialogLayout->addLayout( delStyleLayout );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "PushButton13" );
	cancelButton->setDefault( true );
	okCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( replacementStyleData, SIGNAL(activated(int)), this, SLOT( ReplaceStyle(int) ) );
}

void DelStyle::ReplaceStyle(int id)
{
	replacementStyle = replacementStyleData->text(id);
}

const QString DelStyle::getReplacementStyle()
{
	return replacementStyle;
}

ChooseStyles::ChooseStyles( QWidget* parent, Q3ValueList<ParagraphStyle> *styleList, Q3ValueList<ParagraphStyle> *styleOld)
		: QDialog( parent, "ChooseStyles", true, 0 )
{
	setCaption( tr( "Choose Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	ChooseStylesLayout = new QVBoxLayout( this, 10, 5, "ChooseStylesLayout");
	StyleView = new Q3ListView( this, "StyleView" );
	StyleView->clear();
	StyleView->addColumn( tr( "Available Styles" ) );
	StyleView->header()->setClickEnabled( false, StyleView->header()->count() - 1 );
	StyleView->header()->setResizeEnabled( false, StyleView->header()->count() - 1 );
	StyleView->setSorting(-1);
	int counter = 5;
	bool tabEQ = false;
	for (int x = 5; x < styleList->count(); ++x)
	{
		struct ParagraphStyle vg;
		struct ParagraphStyle vg2;
		vg = (*styleList)[x];
		bool found = false;
		for (int xx=0; xx<styleOld->count(); ++xx)
		{
			vg2 = (*styleOld)[xx];
			if (vg.Vname == vg2.Vname)
			{
				struct PageItem::TabRecord tb;
				tabEQ = false;
				if ((vg2.TabValues.count() == 0) && (vg.TabValues.count() == 0))
					tabEQ = true;
				else
				{
					for (int t1 = 0; t1 < vg2.TabValues.count(); t1++)
					{
						tb.tabPosition = vg2.TabValues[t1].tabPosition;
						tb.tabType = vg2.TabValues[t1].tabType;
						tb.tabFillChar = vg2.TabValues[t1].tabFillChar;
						for (int t2 = 0; t2 < vg.TabValues.count(); t2++)
						{
							struct PageItem::TabRecord tb2;
							tb2.tabPosition = vg.TabValues[t2].tabPosition;
							tb2.tabType = vg.TabValues[t2].tabType;
							tb2.tabFillChar = vg.TabValues[t2].tabFillChar;
							if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
							{
								tabEQ = true;
								break;
							}
						}
						if (tabEQ)
							break;
					}
				}
				if ((vg.LineSpa == vg2.LineSpa) && (vg.Indent == vg2.Indent) && (vg.First == vg2.First) &&
				        (vg.textAlignment == vg2.textAlignment) && (vg.gapBefore == vg2.gapBefore) &&
				        (vg.LineSpaMode == vg2.LineSpaMode) && (vg.gapAfter == vg2.gapAfter) && (vg.Font == vg2.Font) && (tabEQ)
				        && (vg.Drop == vg2.Drop) && (vg.DropDist == vg2.DropDist) && (vg.DropLin == vg2.DropLin) && (vg.FontEffect == vg2.FontEffect) &&
				        (vg.FColor == vg2.FColor) && (vg.FShade == vg2.FShade) && (vg.SColor == vg2.SColor) &&
				        (vg.txtShadowX == vg2.txtShadowX) && (vg.txtShadowY == vg2.txtShadowY) &&
				        (vg.txtOutline == vg2.txtOutline) && (vg.txtUnderPos == vg2.txtUnderPos) && (vg.txtUnderWidth == vg2.txtUnderWidth) &&
				        (vg.scaleH == vg2.scaleH) && (vg.scaleV == vg2.scaleV) && (vg.baseOff == vg2.baseOff) && (vg.kernVal == vg2.kernVal) &&
				        (vg.txtStrikePos == vg2.txtStrikePos) && (vg.txtStrikeWidth == vg2.txtStrikeWidth) &&
				        (vg.SShade == vg2.SShade) && (vg.BaseAdj == vg2.BaseAdj) && (vg.FontSize == vg2.FontSize))
				{
					found = true;
				}
				else
				{
					vg.Vname = "Copy of "+vg2.Vname;
					found = false;
				}
				break;
			}
		}
		if (!found)
		{
			Q3CheckListItem *item = new Q3CheckListItem (StyleView, vg.Vname, Q3CheckListItem::CheckBox);
			item->setOn(true);
			storedStyles.insert(item, counter);
		}
		counter++;
	}
	StyleView->setSorting(0);
	ChooseStylesLayout->addWidget( StyleView );
	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	QSpacerItem* spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer1 );
	OkButton = new QPushButton( CommonStrings::tr_OK, this, "OkButton" );
	layout2->addWidget( OkButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "CancelButton" );
	layout2->addWidget( CancelButton );
	ChooseStylesLayout->addLayout( layout2 );
	resize(230, 280);
	setAttribute( Qt::WA_WState_Polished, false );
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

StilFormate::StilFormate( QWidget* parent, ScribusDoc *doc) : QDialog( parent, "Formate", true, 0)
{
	resize( 327, 260 );
	setCaption( tr( "Edit Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	Docu = doc;
	ReplaceList.clear();
	StilFormateLayout = new QHBoxLayout( this );
	StilFormateLayout->setSpacing( 5 );
	StilFormateLayout->setMargin( 10 );

	ListBox1 = new Q3ListBox( this, "ListBox1" );
	ListBox1->setMinimumSize( QSize( 200, 240 ) );
	StilFormateLayout->addWidget( ListBox1 );

	Layout15 = new QVBoxLayout;
	Layout15->setSpacing( 6 );
	Layout15->setMargin( 0 );

	LoadS = new QPushButton( tr( "&Import" ), this, "LoadF" );
	Layout15->addWidget( LoadS );

	NewB = new QPushButton( tr( "&New" ), this, "NewB" );
	Layout15->addWidget( NewB );

	EditB = new QPushButton( tr( "&Edit" ), this, "EditB" );
	EditB->setDefault( true );
	EditB->setEnabled(false);
	Layout15->addWidget( EditB );

	DublicateB = new QPushButton( tr( "D&uplicate" ), this, "DublicateB" );
	DublicateB->setEnabled(false);
	Layout15->addWidget( DublicateB );

	DeleteB = new QPushButton( tr( "&Delete" ), this, "DeleteB" );
	DeleteB->setEnabled(false);
	Layout15->addWidget( DeleteB );

	SaveB = new QPushButton( CommonStrings::tr_Save, this, "SaveB" );
	Layout15->addWidget( SaveB );

	ExitB = new QPushButton( CommonStrings::tr_OK, this, "ExitB" );
	Layout15->addWidget( ExitB );

	CancelB = new QPushButton( CommonStrings::tr_Cancel, this, "CancelB" );
	Layout15->addWidget( CancelB );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout15->addItem( spacer );
	StilFormateLayout->addLayout( Layout15 );

	// signals and slots connections
	connect(CancelB, SIGNAL(clicked()), this, SLOT(reject()));
	connect(SaveB, SIGNAL(clicked()), this, SLOT(saveIt()));
	connect(ExitB, SIGNAL(clicked()), this, SLOT(accept()));
	connect(EditB, SIGNAL(clicked()), this, SLOT(editFormat()));
	connect(NewB, SIGNAL(clicked()), this, SLOT(neuesFormat()));
	connect(LoadS, SIGNAL(clicked()), this, SLOT(loadStyles()));
	connect(DublicateB, SIGNAL(clicked()), this, SLOT(dupFormat()));
	connect(DeleteB, SIGNAL(clicked()), this, SLOT(deleteFormat()));
	connect(ListBox1, SIGNAL(highlighted(Q3ListBoxItem*)), this, SLOT(selFormat(Q3ListBoxItem*)));
	connect(ListBox1, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selEditFormat(Q3ListBoxItem*)));
	TempVorl = doc->docParagraphStyles;
	UpdateFList();
}

void StilFormate::saveIt()
{
	emit saveStyle(this);
}

void StilFormate::selFormat(Q3ListBoxItem *c)
{
	for (int x = 5; x < TempVorl.count(); ++x)
	{
		if (TempVorl[x].Vname == c->text())
		{
			sFnumber = x;
			break;
		}
	}
	EditB->setEnabled(true);
	DublicateB->setEnabled(true);
	DeleteB->setEnabled(true);
}

void StilFormate::selEditFormat(Q3ListBoxItem *c)
{
	for (int x = 5; x < TempVorl.count(); ++x)
	{
		if (TempVorl[x].Vname == c->text())
		{
			sFnumber = x;
			break;
		}
	}
	EditB->setEnabled(true);
	DublicateB->setEnabled(true);
	DeleteB->setEnabled(true);
	editFormat();
}

void StilFormate::dupFormat()
{
	struct ParagraphStyle sty;
	sty.Vname = tr("Copy of %1").arg(TempVorl[sFnumber].Vname);
	sty.LineSpaMode = TempVorl[sFnumber].LineSpaMode;
	sty.LineSpa = TempVorl[sFnumber].LineSpa;
	sty.textAlignment = TempVorl[sFnumber].textAlignment;
	sty.Indent = TempVorl[sFnumber].Indent;
	sty.First = TempVorl[sFnumber].First;
	sty.gapBefore = TempVorl[sFnumber].gapBefore;
	sty.gapAfter = TempVorl[sFnumber].gapAfter;
	sty.Font = TempVorl[sFnumber].Font;
	sty.FontSize = TempVorl[sFnumber].FontSize;
	sty.TabValues = TempVorl[sFnumber].TabValues;
	sty.Drop = TempVorl[sFnumber].Drop;
	sty.DropLin = TempVorl[sFnumber].DropLin;
	sty.DropDist = TempVorl[sFnumber].DropDist;
	sty.FontEffect = TempVorl[sFnumber].FontEffect;
	sty.FColor = TempVorl[sFnumber].FColor;
	sty.FShade = TempVorl[sFnumber].FShade;
	sty.SColor = TempVorl[sFnumber].SColor;
	sty.SShade = TempVorl[sFnumber].SShade;
	sty.BaseAdj = TempVorl[sFnumber].BaseAdj;
	sty.txtShadowX = TempVorl[sFnumber].txtShadowX;
	sty.txtShadowY = TempVorl[sFnumber].txtShadowY;
	sty.txtOutline = TempVorl[sFnumber].txtOutline;
	sty.txtUnderPos = TempVorl[sFnumber].txtUnderPos;
	sty.txtUnderWidth = TempVorl[sFnumber].txtUnderWidth;
	sty.txtStrikePos = TempVorl[sFnumber].txtStrikePos;
	sty.txtStrikeWidth = TempVorl[sFnumber].txtStrikeWidth;
	sty.scaleH = TempVorl[sFnumber].scaleH;
	sty.scaleV = TempVorl[sFnumber].scaleV;
	sty.baseOff = TempVorl[sFnumber].baseOff;
	sty.kernVal = TempVorl[sFnumber].kernVal;
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true,
	                                static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
}

void StilFormate::neuesFormat()
{
	struct ParagraphStyle sty;
	sty.Vname = tr("New Style");
	sty.LineSpaMode = 0;
	sty.LineSpa = ((Docu->toolSettings.defSize / 10.0) * static_cast<double>(Docu->typographicSettings.autoLineSpacing) / 100) + (Docu->toolSettings.defSize / 10.0);
	sty.textAlignment = 0;
	sty.Indent = 0;
	sty.First = 0;
	sty.gapBefore = 0;
	sty.gapAfter = 0;
	sty.Font = Docu->toolSettings.defFont;
	sty.FontSize = Docu->toolSettings.defSize;
	sty.TabValues.clear();
	sty.Drop = false;
	sty.DropLin = 2;
	sty.DropDist = 0;
	sty.FontEffect = 0;
	sty.FColor = Docu->toolSettings.dBrush;
	sty.FShade = Docu->toolSettings.dShade;
	sty.SColor = Docu->toolSettings.dPen;
	sty.SShade = Docu->toolSettings.dShade2;
	sty.BaseAdj = false;
	sty.txtShadowX = 50;
	sty.txtShadowY = -50;
	sty.txtOutline = 10;
	sty.txtUnderPos = Docu->typographicSettings.valueUnderlinePos;
	sty.txtUnderWidth = Docu->typographicSettings.valueUnderlineWidth;
	sty.txtStrikePos = Docu->typographicSettings.valueStrikeThruPos;
	sty.txtStrikeWidth = Docu->typographicSettings.valueStrikeThruPos;
	sty.scaleH = 1000;
	sty.scaleV = 1000;
	sty.baseOff = 0;
	sty.kernVal = 0;
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true,  static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
}

void StilFormate::editFormat()
{
	EditStyle* dia = new EditStyle(this, &TempVorl[sFnumber], TempVorl, false,
	                               static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	dia->exec();
	delete dia;
	UpdateFList();
}

void StilFormate::deleteFormat()
{
/*	int exit=QMessageBox::warning(this,
	                              CommonStrings::trWarning,
	                              tr("Do you really want to delete this style?"),
	                              tr("No"),
	                              tr("Yes"),
	                              0, 0, 0); */
	/* PFJ - 29.02.04 - Altered to use the correct QMessageBox value. It was 1 */
	/* FS - 13.03.04 the 1 is correct in this version of QMessageBox, it returns the Nr of the clicked Button either 0 or 1 or 2 */
//	if (exit == 1)
	DelStyle *dia = new DelStyle(this, TempVorl, TempVorl[sFnumber].Vname);
	if (dia->exec())
	{
		if (ReplaceList.values().contains(TempVorl[sFnumber].Vname))
		{
			QMap<QString,QString>::Iterator it;
			for (it = ReplaceList.begin(); it != ReplaceList.end(); ++it)
			{
				if (it.data() == TempVorl[sFnumber].Vname)
					it.data() = dia->getReplacementStyle();
			}
		}
		ReplaceList.insert(TempVorl[sFnumber].Vname, dia->getReplacementStyle());
		ListBox1->removeItem(sFnumber);
		TempVorl.remove(TempVorl.at(sFnumber));
		UpdateFList();
	}
	delete dia;
}

void StilFormate::loadStyles()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("editformats", ".");
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("editformats", selectedFile.left(selectedFile.findRev("/")));
		Q3ValueList<ParagraphStyle> TempVorl2;
		for (uint x = 0; x < 5; ++x)
		{
			TempVorl2.append(TempVorl[x]);
		}
		Docu->loadStylesFromFile(selectedFile, &TempVorl2);
		ChooseStyles* dia2 = new ChooseStyles(this, &TempVorl2, &TempVorl);
		if (dia2->exec())
		{
			QStringList neededColors;
			neededColors.clear();
			QMap<Q3CheckListItem*, int>::Iterator it;
			for (it = dia2->storedStyles.begin(); it != dia2->storedStyles.end(); ++it)
			{
				struct ParagraphStyle sty;
				if (it.key()->isOn())
				{
					sty = TempVorl2[it.data()];
					TempVorl.append(sty);
					if ((!Docu->PageColors.contains(sty.SColor)) && (!neededColors.contains(sty.SColor)))
						neededColors.append(sty.SColor);
					if ((!Docu->PageColors.contains(sty.FColor)) && (!neededColors.contains(sty.FColor)))
						neededColors.append(sty.FColor);
				}
			}
			if (!neededColors.isEmpty())
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				if (ss->ReadColors(selectedFile))
				{
					ColorList LColors = ss->Farben;
					ColorList::Iterator itc;
					for (itc = LColors.begin(); itc != LColors.end(); ++itc)
					{
						if (neededColors.contains(itc.key()))
							Docu->PageColors.insert(itc.key(), itc.data());
					}
				}
				delete ss;
			}
		}
		UpdateFList();
	}
	else
		return;
}

void StilFormate::UpdateFList()
{
	ListBox1->clear();
	if (TempVorl.count() < 6)
		return;
	for (int x = 5; x < TempVorl.count(); ++x)
		ListBox1->insertItem(TempVorl[x].Vname);
	if (ListBox1->currentItem() == -1)
	{
		DublicateB->setEnabled(false);
		EditB->setEnabled(false);
		DeleteB->setEnabled(false);
	}
	ListBox1->sort( true );
	ListBox1->setSelected(ListBox1->currentItem(), false);
}
