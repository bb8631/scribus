#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <qdom.h>
#include <q3ptrstack.h>
#include <Q3ValueList>
#include <Q3PtrList>
#include <QMap>
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "vgradient.h"

class ScrAction;

class PLUGIN_API SVGImportPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		SVGImportPlugin();
		virtual ~SVGImportPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;

	public slots:
		virtual bool import(QString target = QString::null);

	private:
		void registerFormats();
		ScrAction* importAction;

};

extern "C" PLUGIN_API int svgimplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* svgimplugin_getPlugin();
extern "C" PLUGIN_API void svgimplugin_freePlugin(ScPlugin* plugin);

class PageItem;
class ScribusDoc;
class PrefsManager;
class FPointArray;

class GradientHelper
{
public:
	GradientHelper()
		{
		Type = 1;
		typeValid = false;
		CSpace = false;
		cspaceValid = true;
		X1 = 0;
		x1Valid = true;
		X2 = 1;
		x2Valid = true;
		Y1 = 0;
		y1Valid = true;
		Y2 = 0;
		y2Valid = true;
		gradient = VGradient(VGradient::linear);
		gradientValid = false;
		matrix = QMatrix();
		matrixValid = false;
		reference = "";
		}
	int Type;
	bool typeValid;
	QString reference;
	VGradient gradient;
	bool gradientValid;
	double X1;
	bool x1Valid;
	double Y1;
	bool y1Valid;
	double X2;
	bool x2Valid;
	double Y2;
	bool y2Valid;
	bool CSpace;
	bool cspaceValid;
	QMatrix	matrix;
	bool matrixValid;
	};

class SvgStyle
{
public:
	SvgStyle()
		{
		LWidth = 1.0;
		Transparency = 0.0;
		TranspStroke = 0.0;
		StrokeCol = "None";
		FillCol = "Black";
		CurCol = "None";
		Gradient = 0;
		GCol1 = "Black";
		GCol2 = "Black";
		GX1 = 0;
		GY1 = 0;
		GX2 = 0;
		GY2 = 0;
		GradCo = VGradient(VGradient::linear);
		CSpace = false;
		Family = "";
		FontSize = 12;
		matrix = QMatrix();
		matrixg = QMatrix();
		PLineArt = Qt::SolidLine;
		PLineJoin = Qt::MiterJoin;
		PLineEnd = Qt::FlatCap;
		InherCol = false;
		dashOffset = 0;
		dashArray.clear();
		}
	QMatrix	matrix;
	QMatrix	matrixg;
	double LWidth;
	Qt::PenStyle PLineArt;
	Qt::PenJoinStyle PLineJoin;
	Qt::PenCapStyle PLineEnd;
	QString StrokeCol;
	QString FillCol;
	QString CurCol;
	QString GCol1;
	QString GCol2;
	double GX1;
	double GY1;
	double GX2;
	double GY2;
	int Gradient;
	VGradient	GradCo;
	bool CSpace;
	int FontSize;
	QString Family;
	double Transparency;
	double TranspStroke;
	bool InherCol;
	double dashOffset;
	Q3ValueList<double> dashArray;
};

class SVGPlug : public QObject
{
	Q_OBJECT

public:
	SVGPlug(QString fname, bool isInteractive);
	~SVGPlug();
	void convert();
	void addGraphicContext();
	void setupTransform( const QDomElement &e );
	Q3PtrList<PageItem> parseGroup(const QDomElement &e);
	double fromPercentage( const QString &s );
	double parseUnit(const QString &unit);
	QMatrix parseTransform(const QString &transform);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag);
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);
	QColor parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void parsePA( SvgStyle *obj, const QString &command, const QString &params );
	void parseStyle( SvgStyle *obj, const QDomElement &e );
	void parseColorStops(GradientHelper *gradient, const QDomElement &e);
	void parseGradient( const QDomElement &e );
	QPointF GetMaxClipO(FPointArray Clip);
	QPointF GetMinClipO(FPointArray Clip);
	Q3PtrList<PageItem> parseText(double x, double y, const QDomElement &e);

	ScribusDoc* currDoc;
	QDomDocument inpdoc;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;
	Q3PtrList<PageItem> Elements;
	Q3PtrStack<SvgStyle>	m_gc;
	QMap<QString, GradientHelper>	m_gradients;
	bool FirstM, WasM, PathClosed;
	double viewTransformX;
	double viewTransformY;
	double viewScaleX;
	double viewScaleY;
	bool haveViewBox;
	bool interactive;
};

#endif
