#pragma once
#include <qmessagebox.h>
#include "qgsgeometry.h"
#include "qgslinestringv2.h"
#include "qgsmapcanvas.h"
#include "qgsproject.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include "qobject.h"
class SilenEditUtils: public QObject
{
	Q_OBJECT
public:
	SilenEditUtils(void);
	void processGeometry(QgsVectorLayer *vlayer,const QgsGeometry* drawGeo,int opType);
	void saveGeometry(QgsVectorLayer *vlayer,const QgsGeometry* geos);
	bool changeFeatureGeo(QgsVectorLayer *vlayer,int fid,QgsGeometry* inGeo,bool changed);
	~SilenEditUtils(void);
};

