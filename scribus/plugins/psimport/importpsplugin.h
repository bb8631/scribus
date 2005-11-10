#ifndef IMPORTPSPLUGIN_H
#define IMPORTPSPLUGIN_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
//Added by qt3to4:
#include <Q3ValueList>

class ScrAction;

class PLUGIN_API ImportPSPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ImportPSPlugin();
		virtual ~ImportPSPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;

	public slots:
		// Import menu item
		virtual bool import(QString target = QString::null);

	private:
		void registerFormats();
		ScrAction* importAction;
};

extern "C" PLUGIN_API int importps_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importps_getPlugin();
extern "C" PLUGIN_API void importps_freePlugin(ScPlugin* plugin);

#endif
