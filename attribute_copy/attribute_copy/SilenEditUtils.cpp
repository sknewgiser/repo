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
///=============================Utils��====================================
	//������Ƶ�Ҫ��
void SilenEditUtils::processGeometry(QgsVectorLayer *vlayer,const QgsGeometry* drawGeo,int opType)
{
	if(!vlayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","ͼ����봦�ڿɱ༭״̬��");
		return;
	}
	//int opType =0;//�������ͣ�0.������1.�У�2.�ָ�
	if(!vlayer || !drawGeo)return ;
	//��ȡ�û����ƵĶ���Σ�Ȼ��ӵ�ǰͼ���л�ȡ�ཻ��Ҫ��
	QgsRectangle bBox = drawGeo->boundingBox();
	QgsFeatureRequest fr ;
	fr= fr.setFilterRect( bBox );
	fr.setFlags( QgsFeatureRequest::ExactIntersect );

	QgsFeatureIterator fit =  vlayer->getFeatures(fr );
	int count(0);
	QgsFeature f;
	//�ȸ��ݾ��ο��ȡҪ����Ŀ��һ�㲻���鳬��5������Σ��������5��������û����ƵĶ����̫��
	while ( fit.nextFeature( f ) )
	{
		QgsGeometry* g = f.geometry();
		if(!g->isEmpty() && g->intersects(drawGeo))
			count++;
	}
	if(count <= 0)//������ǰ�༭ͼ����û����֮�ཻ����ֱ���½�,���ﵽʱ��Ҫ��ӹ�����߲���Ҫ����ֱ�ӷ���
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*drawGeo); //�����û����Ƶ�ͼ��
		vlayer->editBuffer()->addFeature(nFeature);
		//this->mCanvas->refresh();
		return ;
	}
	//int opType =0;//�������ͣ�0.������1.�У�2.�ָ�
	//QString msg;
	//if(count>=1)
	//{
	//	//msg =tr("There are %1 polygons intersect with your draw,Do you want to:").arg(count);
	//	//QMessageBox msgBox(QMessageBox::Question,tr("Add Polygon"),msg);
	//	//msgBox.setStandardButtons( QMessageBox::Yes|QMessageBox::YesAll|QMessageBox::Cancel);
	//	//msgBox.setDefaultButton(QMessageBox::Yes);
	//	//msgBox.setButtonText( QMessageBox::Yes,tr("Clip"));//ָʹ�û��ƶ���ν�����ͼ�ν����и
	//	//msgBox.setButtonText(QMessageBox::YesAll,tr("Split"));//ָ���ڱ���ͼ�ζԻ���ͼ�ν��зָ�
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
		//1.����Ҫ�صļ���ͼ�����û����Ƶļ���ͼ�β���ͬ�Ĳ������ڸ��µ�ǰҪ�صļ��β���
		//��������Զ���Ķ�ĳЩ�ֶν��а���������������
		QgsGeometry* fGeo = g->difference(drawGeo); //����Ҫ��ͼ�������ͼ�β��ص�����
		//g->(drawGeo);
		QgsGeometry* iterGeos = drawGeo->intersection(g);
		//2.�Լ�������Ĳ���ͬ����ͼ�ν��������ж��봦����������Ŀ��ͼ��һ��ʱ�������ݱ���
		if(fGeo && !fGeo->isEmpty())//������첿��ͼ�δ�������Ҫ����ԭ��Ҫ��ͼ��
		{
			bool changed = SilenEditUtils::changeFeatureGeo(vlayer, f.id(), fGeo,false);
		}
		if(!iterGeos || iterGeos->isEmpty())continue;
		//�����Clip�򣬶Ի���ͼ����ȫ����Ҫ�ؽ���ɾ������
		if(opType ==1){
			if(drawGeo->contains(g))
				vlayer->editBuffer()->deleteFeature(f.id());
			continue;
		}
		if(opType==2)//�½��ཻ���ֵļ���ͼ��Ϊһ��Ҫ��
			SilenEditUtils::saveGeometry(vlayer,iterGeos);
		//�����û����ƵĶ��ಿ��ͼ��,�������κ�ͼ���ཻ�Ĳ���ͼ�Σ�ֱ�ӱ���Ϊ�µ�Ҫ��
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
		//nFeature.setGeometry(*drawGeo); //�����û����Ƶ�ͼ��
		//vlayer->editBuffer()->addFeature(nFeature);
	}
	if(diffGeo && !diffGeo->isEmpty())//���ʣ�µ�ͼ�β�Ϊ����ֱ�Ӵ���Ҫ��
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*diffGeo); //�����û����Ƶ�ͼ��
		vlayer->editBuffer()->addFeature(nFeature);
	}
	vlayer->endEditCommand();
}
//�½�Ҫ�ر�������
void SilenEditUtils::saveGeometry(QgsVectorLayer *vlayer,const QgsGeometry* geos)
{
	QGis::WkbType lyrGeoType = vlayer->wkbType();
	QGis::WkbType geoType = geos->wkbType();
	//����ֻ��WkbPolygon��WkbMultiPolygon���з�����������ݱ���Ĺ���
	if(geoType == lyrGeoType)//�������ͼ�ε�������ͼ����ͬ��ֱ�ӱ���
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*geos); //�����û����Ƶ�ͼ��
		vlayer->editBuffer()->addFeature(nFeature);
		return ;
	}
	else if(geoType == QGis::WKBPolygon && lyrGeoType == QGis::WKBMultiPolygon)
	{
		QgsFeature nFeature;
		nFeature.initAttributes(vlayer->fields().count());
		nFeature.setGeometry(*geos); //�����û����Ƶ�ͼ��
		vlayer->editBuffer()->addFeature(nFeature);
		return ;
	}
	else if(geoType == QGis::WKBMultiPolygon && lyrGeoType == QGis::WKBPolygon )
	{
		QList<QgsGeometry*> inGeos = geos->asGeometryCollection();
		if(inGeos.count() <=0 )return ;
		Q_FOREACH(QgsGeometry* geo,inGeos)//ѭ�����еļ���ͼ�λ�ȡ��Ҫ��ͼ������
		{
			if(!geo->isEmpty()){
				QGis::WkbType geoType2 = geo->wkbType();
				if(geoType2 == lyrGeoType)//���������������ͬ��ֱ�ӱ��棬������Ҫ�ֽ�Ϊ�Ӳ��ֽ��д���
				{
					QgsFeature nFeature;
					nFeature.initAttributes(vlayer->fields().count());
					nFeature.setGeometry(*geos); //�����û����Ƶ�ͼ��
					vlayer->editBuffer()->addFeature(nFeature);
					return ;
				}
				else 
					if (geoType2 == QGis::WKBMultiPolygon)//�ಿ�������ֽ�ʹ���Feature����
						saveGeometry(vlayer,geo);
			}
		}
	}
}
//�޸�Ҫ�صļ���ͼ��
bool SilenEditUtils::changeFeatureGeo(QgsVectorLayer *vlayer,int fid,QgsGeometry* inGeo,bool changed)
{
	
	if(!inGeo || inGeo->isEmpty())return false;
	QGis::WkbType lyrGeoType = vlayer->wkbType();
	QGis::WkbType geoType = inGeo->wkbType();

	if(geoType == lyrGeoType)//��������ͼ����ͼ���ͼ��һ����ֱ���滻Ҫ�صļ���ͼ��
	{
		vlayer->editBuffer()->changeGeometry( fid, inGeo);
		return true;
	}
	//���Ҫ��Ϊ
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
		bool changeOld = changed;//Ҫ�صļ��������Ƿ��Ѿ�����
		Q_FOREACH(QgsGeometry* geo,inGeos)//ѭ�����еļ���ͼ�λ�ȡ��Ҫ��ͼ������
		{
			if(!geo->isEmpty()){
				QGis::WkbType geoType2 = geo->wkbType();
				if(geoType2 == lyrGeoType)//���������������ͬ��ֱ�ӱ��棬������Ҫ�ֽ�Ϊ�Ӳ��ֽ��д���
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
						nFeature.setGeometry(*geo); //�����û����Ƶ�ͼ��
						vlayer->editBuffer()->addFeature(nFeature);
					}
				}
				else 
					if (geoType2 == QGis::WKBMultiPolygon)//�ಿ�������ֽ�ʹ���Feature����
						changeOld =changeFeatureGeo(vlayer,fid,geo,changeOld);//  saveGeometry(vlayer,geo);
			}
		}
		return changeOld;
	}
	return false;
}