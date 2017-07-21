#include "SilenEditUtils.h"
#include <qmessagebox.h>
#include "qgsgeometry.h"
#include "qgslinestringv2.h"
#include "qgsmapcanvas.h"
#include "qgsproject.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include "qobject.h"

#include "qgspointv2.h"
#include "qgspolygonv2.h"
#include "qgsvectorlayereditbuffer.h"

#include "qgsrubberband.h"
#include "qgis.h"
SilenEditUtils::SilenEditUtils(void)
{
}


SilenEditUtils::~SilenEditUtils(void)
{
}
///=============================Utils类====================================
	//处理绘制的要素
void SilenEditUtils::processGeometry(QgsVectorLayer *vlayer,const QgsGeometry* drawGeo,int opType)
{
	if(!vlayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","图层必须处于可编辑状态！");
		return;
	}
	//int opType =0;//操作类型，0.新增，1.切，2.分割
	if(!vlayer || !drawGeo)return ;
	//获取用户绘制的多边形，然后从当前图层中获取相交的要素
	QgsRectangle bBox = drawGeo->boundingBox();
	QgsFeatureRequest fr ;
	fr= fr.setFilterRect( bBox );
	fr.setFlags( QgsFeatureRequest::ExactIntersect );

	QgsFeatureIterator fit =  vlayer->getFeatures(fr );
	int count(0);
	QgsFeature f;
	//先根据矩形框获取要素数目，一般不建议超过5个多边形，如果超过5个则表明用户绘制的多边形太大
	while ( fit.nextFeature( f ) )
	{
		QgsGeometry* g = f.geometry();
		if(!g->isEmpty() && g->intersects(drawGeo))
			count++;
	}
	if(count <= 0)//表明当前编辑图层中没有与之相交，则直接新建,这里到时需要添加规则或者不需要保存直接返回
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*drawGeo); //保存用户绘制的图形
		vlayer->editBuffer()->addFeature(nFeature);
		//this->mCanvas->refresh();
		return ;
	}
	//int opType =0;//操作类型，0.新增，1.切，2.分割
	//QString msg;
	//if(count>=1)
	//{
	//	//msg =tr("There are %1 polygons intersect with your draw,Do you want to:").arg(count);
	//	//QMessageBox msgBox(QMessageBox::Question,tr("Add Polygon"),msg);
	//	//msgBox.setStandardButtons( QMessageBox::Yes|QMessageBox::YesAll|QMessageBox::Cancel);
	//	//msgBox.setDefaultButton(QMessageBox::Yes);
	//	//msgBox.setButtonText( QMessageBox::Yes,tr("Clip"));//指使用绘制多边形将本底图形进行切割，
	//	//msgBox.setButtonText(QMessageBox::YesAll,tr("Split"));//指基于本底图形对绘制图形进行分割
	//	/*int ret=msgBox.exec();
	//	switch(ret)
	//	{
	//	case QMessageBox::Cancel:
	//		return ;
	//	case QMessageBox::Yes:
 //			opType = 1;
	//		break;
	//	case QMessageBox::YesAll:
	//		opType=2;
	//		break;
	//	default:opType=0;
	//	}*/
	//}
	vlayer->beginEditCommand( tr( "Feature added" ) );		
	QgsGeometry newGeo;
	QgsFeatureIterator fit2 =  vlayer->getFeatures(fr);
	QgsGeometry * diffGeo(nullptr);
	while(fit2.nextFeature(f))
	{
		if ( !f.constGeometry() )
			continue;
		QgsGeometry* g = f.geometry();
		if(g->isEmpty() || !g->intersects(drawGeo))continue;
		//1.计算要素的几何图形与用户绘制的几何图形不相同的部分用于更新当前要素的几何部分
		//这里可以自定义的对某些字段进行按面积比例分配计算
		QgsGeometry* fGeo = g->difference(drawGeo); //计算要素图形与绘制图形不重叠部分
		//g->(drawGeo);
		QgsGeometry* iterGeos = drawGeo->intersection(g);
		//2.对计算出来的不相同部分图形进行类型判断与处理，当类型与目标图层一致时进行数据保存
		if(fGeo && !fGeo->isEmpty())//如果差异部分图形存在则需要更新原来要素图形
		{
			bool changed = SilenEditUtils::changeFeatureGeo(vlayer, f.id(), fGeo,false);
		}
		if(!iterGeos || iterGeos->isEmpty())continue;
		//如果是Clip则，对绘制图形完全包括要素进行删除处理
		if(opType ==1){
			if(drawGeo->contains(g))
				vlayer->editBuffer()->deleteFeature(f.id());
			continue;
		}
		if(opType==2)//新建相交部分的几何图形为一个要素
			SilenEditUtils::saveGeometry(vlayer,iterGeos);
		//计算用户绘制的多余部分图形,即不与任何图形相交的部分图形，直接保存为新的要素
		if(diffGeo&&!diffGeo->isEmpty())
		{
			QgsGeometry* tmp =  diffGeo->difference(g);
			delete diffGeo;
			diffGeo = tmp;
		}
		else diffGeo = drawGeo->difference(g);
	}
	if(opType==1)
	{
		SilenEditUtils::saveGeometry(vlayer,drawGeo);
		//QgsFeature nFeature;
		//nFeature.initAttributes(vlayer->fields().count());
		//nFeature.setGeometry(*drawGeo); //保存用户绘制的图形
		//vlayer->editBuffer()->addFeature(nFeature);
	}
	if(diffGeo && !diffGeo->isEmpty())//如果剩下的图形不为空则直接创建要素
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*diffGeo); //保存用户绘制的图形
		vlayer->editBuffer()->addFeature(nFeature);
	}
	vlayer->endEditCommand();
}
//新建要素保存数据
void SilenEditUtils::saveGeometry(QgsVectorLayer *vlayer,const QgsGeometry* geos)
{
	QGis::WkbType lyrGeoType = vlayer->wkbType();
	QGis::WkbType geoType = geos->wkbType();
	//这里只对WkbPolygon和WkbMultiPolygon进行分析处理和数据保存的工作
	if(geoType == lyrGeoType)//如果几何图形的类型与图层相同则直接保存
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*geos); //保存用户绘制的图形
		vlayer->editBuffer()->addFeature(nFeature);
		return ;
	}
	else if(geoType == QGis::WKBPolygon && lyrGeoType == QGis::WKBMultiPolygon)
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*geos); //保存用户绘制的图形
		vlayer->editBuffer()->addFeature(nFeature);
		return ;
	}
	else if(geoType == QGis::WKBMultiPolygon && lyrGeoType == QGis::WKBPolygon )
	{
		QList<QgsGeometry*> inGeos = geos->asGeometryCollection();
		if(inGeos.count() <=0 )return ;
		Q_FOREACH(QgsGeometry* geo,inGeos)//循环所有的几何图形获取需要的图形数据
		{
			if(!geo->isEmpty()){
				QGis::WkbType geoType2 = geo->wkbType();
				if(geoType2 == lyrGeoType)//如果两个的类型相同则直接保存，否则需要分解为子部分进行处理
				{
					QgsFeature nFeature;
					nFeature.initAttributes(vlayer->fields().count());
					nFeature.setGeometry(*geos); //保存用户绘制的图形
					vlayer->editBuffer()->addFeature(nFeature);
					return ;
				}
				else 
					if (geoType2 == QGis::WKBMultiPolygon)//多部多边形则分解和创建Feature保存
						saveGeometry(vlayer,geo);
			}
		}
	}
}
//修改要素的几何图形
bool SilenEditUtils::changeFeatureGeo(QgsVectorLayer *vlayer,int fid,QgsGeometry* inGeo,bool changed)
{
	
	if(!inGeo || inGeo->isEmpty())return false;
	QGis::WkbType lyrGeoType = vlayer->wkbType();
	QGis::WkbType geoType = inGeo->wkbType();

	if(geoType == lyrGeoType)//如果输入的图形与图层的图形一致则直接替换要素的几何图形
	{
		vlayer->editBuffer()->changeGeometry( fid, inGeo);
		return true;
	}
	//如果要素为
	else if(geoType == QGis::WKBPolygon && lyrGeoType == QGis::WKBMultiPolygon)
	{
		QgsGeometry* mgeo = QgsGeometry::fromMultiPolygon(inGeo->asMultiPolygon());
		if(mgeo)
			vlayer->editBuffer()->changeGeometry( fid, mgeo);
		return true;
	}
	else if(geoType == QGis::WKBMultiPolygon && lyrGeoType == QGis::WKBPolygon )
	{
		QList<QgsGeometry*> inGeos = inGeo->asGeometryCollection();
		if(inGeos.count() <= 0 )return false;
		bool changeOld = changed;//要素的几何数据是否已经修正
		Q_FOREACH(QgsGeometry* geo,inGeos)//循环所有的几何图形获取需要的图形数据
		{
			if(!geo->isEmpty()){
				QGis::WkbType geoType2 = geo->wkbType();
				if(geoType2 == lyrGeoType)//如果两个的类型相同则直接保存，否则需要分解为子部分进行处理
				{
					if(!changeOld)
					{
						vlayer->editBuffer()->changeGeometry( fid, geo);
						changeOld = true;
					}
					else
					{
						QgsFeature nFeature;
						nFeature.initAttributes(vlayer->fields().count());
						nFeature.setGeometry(*geo); //保存用户绘制的图形
						vlayer->editBuffer()->addFeature(nFeature);
					}
				}
				else 
					if (geoType2 == QGis::WKBMultiPolygon)//多部多边形则分解和创建Feature保存
						changeOld =changeFeatureGeo(vlayer,fid,geo,changeOld);//  saveGeometry(vlayer,geo);
			}
		}
		return changeOld;
	}
	return false;
}