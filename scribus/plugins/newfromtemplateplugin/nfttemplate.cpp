/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "nfttemplate.h"
#include <qfileinfo.h>
//Added by qt3to4:
#include <QTextStream>

nfttemplate::nfttemplate(QFile* tmplXmlFile, const QString &tmplCategory) 
{
	tmplXml = tmplXmlFile;
	templateCategory = tmplCategory;
	isWritable = tmplXml->open(QIODevice::WriteOnly | QIODevice::ReadOnly);
	tmplXml->close();
	isDeleted = false;
}

void nfttemplate::remove()
{
	if (tmplXml->exists())
	{
		QString newTmplXml = "";
		QString tmp;
		bool collect = false;
		tmplXml->open(QIODevice::ReadOnly);
		QTextStream stream(tmplXml);
		QString line = stream.readLine();
		while (!line.isNull())
		{
			if ((line.find(enCategory) != -1) || collect)
			{
				collect = true;
				line += "\n";
				tmp += line;
				if (line.find("name") != -1)
				{
					if (line.find(name) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.find("file") != -1)
				{
					QString shortFile = file.right(file.length() - file.findRev("/") -1);
					if (line.find(shortFile) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.find("</template>") != -1)
				{
					collect = false;
					tmp = "";
				}
			}
			else
			{
				line += "\n";
				newTmplXml += line;
			}
			line = stream.readLine();
		}
		tmplXml->close();
		tmplXml->open(QIODevice::WriteOnly);
		QTextStream instream(tmplXml);
		instream.setEncoding(QTextStream::UnicodeUTF8);
		instream << newTmplXml;
		tmplXml->close();
	}
}

bool nfttemplate::canWrite()
{
	return isWritable;
}

bool nfttemplate::isValid()
{
	bool ret = true;
	
	QFileInfo *fi = new QFileInfo(file);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	fi = new QFileInfo(tnail);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	fi = new QFileInfo(img);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	return ret;	
}

nfttemplate::~nfttemplate() 
{
	if (isDeleted)
		remove();
}
