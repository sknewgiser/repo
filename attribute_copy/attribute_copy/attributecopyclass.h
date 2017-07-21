#pragma once
#include <qgsfeature.h>
#include <qmap.h>
#include <qgsfield.h>
#include <qgsspatialindex.h>
#include <qgsvectorlayer.h>
#include <qtablewidget.h>
#include <qcombobox.h>
class attributecopyclass: public QObject
{
public:
	attributecopyclass(void);
	
	void copyLyrToLyr(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap,int opType);
	void splitCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap);
	void clipCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap);
	QgsFeature copyattributes(QgsFeature srcFeature,QgsFields *srcFields,QgsFeature destFeature,QgsFields *destFields,QMap<QString,QString> *fieldMap);
	void updateFeature(QgsFeature &feature,QgsVectorLayer *mLayer);
	~attributecopyclass(void);
private:
	QMutex mLayerMutex;
    QMutex mIndexMutex;
    QgsSpatialIndex mIndex;
	int getAreaMax(QList<double> areaList);

};

