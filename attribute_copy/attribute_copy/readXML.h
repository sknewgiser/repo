#pragma once
#include <QtGui/QMainWindow>
#include <qdom.h>
#include <qpair.h>
#include <qtablewidget.h>
class readXML
{
public:
	readXML(void);
	QStringList getNodesTagName(QString filePath);
	QList<QPair<QString,QStringList>> readXml(QString filePath,QString selectedTagName);
	void writeXml(QString filePath,QTableWidget *table,QString nodeTagName,QString srcLayerName,QString targetLayerName);
	void saveCurrentNode(QString filePath,QTableWidget *table,QString nodeTagName,QString srcLayerName,QString targetLayerName);
	void deleteNode(QString filePath,QString nodeTagName);
	~readXML(void);
};

