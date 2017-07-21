/***************************************************************************
    qgsspatialitefeatureiterator.cpp
    ---------------------
    begin                : Juli 2012
    copyright            : (C) 2012 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "attributecopyclass.h"
#include "qstringlist.h"
#include "qgsgeometry.h"
#include <qgsfeature.h>
#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qxmlstream.h>
#include <qcombobox.h>
#include <qobject.h>
#include <qdebug.h>
#include <SilenEditUtils.h>
attributecopyclass::attributecopyclass(void)
{
}
//��ͼ������Կ���
void attributecopyclass::copyLyrToLyr(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap,int opType)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","ͼ����봦�ڿɱ༭״̬��");
		return;
	}
	if(!srcLayer->isValid()||!targetLayer->isValid()) return;
	QgsFeatureIterator srcIter;
	QgsFeatureIterator targetIter;
	if(srcLayer->selectedFeatureCount()>0)
	{
		srcIter=srcLayer->selectedFeaturesIterator();
	}
	else
	{
		srcIter=srcLayer->getFeatures();
		//QMessageBox::about(nullptr,"Error","��ѡ�����ڽ��п�����Դͼ��С�࣡");
	}
	QList<QgsFeature> srcFList;
	QList<QgsFeature> targetFList;
	QList<double> areaList;

	QgsFields *srcFields;
	QgsFields *targetFields;
	srcFields=new QgsFields(srcLayer->fields());
	targetFields=new QgsFields(targetLayer->fields());

	QgsFeature srcFeature;	
	SilenEditUtils silen;
	
	while(srcIter.nextFeature(srcFeature))
	{	
		silen.processGeometry(targetLayer,srcFeature.geometry(),opType);
		areaList.clear();
		srcFList.clear();
		targetFList.clear();
		QgsFeatureRequest request=QgsFeatureRequest().setFilterRect(srcFeature.geometry()->boundingBox());
		targetIter=targetLayer->getFeatures(request);	
		QgsFeature targetFeature;
		while(targetIter.nextFeature(targetFeature))
		{	
			
			if(srcFeature.geometry()->intersects(targetFeature.geometry()))
			{

				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//��ͼ������ཻ��������
				//double targetArea=targetFeature.geometry()->area();//����ͼ��ĳ��������
				if(intersectArea>0.1)//������������趨����ֵ����и�ֵ
				{
					targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//��Դͼ��Feature�����ֶ����Ե�Ŀ��ͼ��Feature
					this->updateFeature(targetFeature,targetLayer);//Feature���±���		
					
				}

				///*ͼ�����Ա���mian_ji��λ�ǹ��꣬1����=10000ƽ����*/
				//double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//��ͼ������ཻ��������
				////double targetArea=targetFeature.geometry()->area();//����ͼ��ĳ��������
				//double srcArea=srcFeature.geometry()->area();//Դͼ��������
				//double coverageRate=(intersectArea/srcArea)*100;//�ཻ�������ռԴͼ���Ӧ������ı���

				/*areaList.append(intersectArea);
				srcFList.append(srcFeature);
				targetFList.append(targetFeature);*/
			}
	    
		}
	//	int index=getAreaMax(areaList);//��ȡ�ཻ�������������ֵ
		//srcFeature=srcFList.at(index);//��ȡ�ཻ���������
	//	targetFeature=targetFList.at(index);
		
	}
	QMessageBox::about(nullptr,QObject::tr("��ʾ"),QObject::tr("������ɣ�"));
}
//�ü�ģʽ����
void attributecopyclass::clipCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","ͼ����봦�ڿɱ༭״̬��");
		return;
	}
	if(!srcLayer->isValid()||!targetLayer->isValid()) return;
	QgsFeatureIterator srcIter;
	QgsFeatureIterator targetIter;
	if(targetLayer->selectedFeatureCount()>0)
	{
		targetIter=targetLayer->selectedFeaturesIterator();
	}
	else
	{
		targetIter=targetLayer->getFeatures();
	}
	QgsVectorDataProvider *vectorProvider=srcLayer->dataProvider();
	QList<QgsFeature> srcFList;
	QList<QgsFeature> targetFList;
	QList<double> areaList;
	
	QgsFields *srcFields;
	QgsFields *targetFields;
	srcFields=new QgsFields(vectorProvider->fields());
	targetFields=new QgsFields(targetLayer->fields());

	QgsFeature targetFeature;
	while(targetIter.nextFeature(targetFeature))
	{	
		areaList.clear();
		srcFList.clear();
		targetFList.clear();
		QgsFeatureRequest request=QgsFeatureRequest().setFilterRect(targetFeature.geometry()->boundingBox());
		srcIter=vectorProvider->featureSource()->getFeatures(request);	
		QgsFeature srcFeature;
		while(srcIter.nextFeature(srcFeature))
		{	
			if(srcFeature.geometry()->intersects(targetFeature.geometry()))
			{
				srcFList.append(srcFeature);
				targetFList.append(targetFeature);
				///*ͼ�����Ա���mian_ji��λ�ǹ��꣬1����=10000ƽ����*/
				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//��ͼ������ཻ��������
				areaList.append(intersectArea);
			}
		}
		int index=getAreaMax(areaList);//��ȡ�ཻ�������������ֵ
		srcFeature=srcFList.at(index);//��ȡ�ཻ���������
		targetFeature=targetFList.at(index);
		targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//��Դͼ��Feature�����ֶ����Ե�Ŀ��ͼ��Feature
		this->updateFeature(targetFeature,targetLayer);//Feature���±���
	}
	QMessageBox::about(nullptr,QObject::tr("��ʾ"),QObject::tr("������ɣ�"));
}
//�ָ�ģʽ����
void attributecopyclass::splitCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","ͼ����봦�ڿɱ༭״̬��");
		return;
	}
	if(!srcLayer->isValid()||!targetLayer->isValid()) return;
	QgsFeatureIterator srcIter;
	QgsFeatureIterator targetIter;
	if(targetLayer->selectedFeatureCount()>0)
	{
		targetIter=targetLayer->selectedFeaturesIterator();
	}
	else
	{
		targetIter=targetLayer->getFeatures();
	}
	QgsVectorDataProvider *vectorProvider=srcLayer->dataProvider();

	QgsFields *srcFields;
	QgsFields *targetFields;
	srcFields=new QgsFields(vectorProvider->fields());
	targetFields=new QgsFields(targetLayer->fields());

	QgsFeature targetFeature;
	while(targetIter.nextFeature(targetFeature))
	{	
		QgsFeatureRequest request=QgsFeatureRequest().setFilterRect(targetFeature.geometry()->boundingBox());
		srcIter=vectorProvider->featureSource()->getFeatures(request);	
		QgsFeature srcFeature;
		while(srcIter.nextFeature(srcFeature))
		{			
			if(srcFeature.geometry()->intersects(targetFeature.geometry()))
			{
				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//��ͼ������ཻ��������
				//double targetArea=targetFeature.geometry()->area();//����ͼ��ĳ��������
				if(intersectArea>0.1)//������������趨����ֵ����и�ֵ
				{
					targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//��Դͼ��Feature�����ֶ����Ե�Ŀ��ͼ��Feature
					this->updateFeature(targetFeature,targetLayer);//Feature���±���		
					
				}
			}
	    
		}
		
	}
	QMessageBox::about(nullptr,QObject::tr("��ʾ"),QObject::tr("������ɣ�"));
}

//��ȡ���ཻ������ֵ��Ӧ������
int attributecopyclass::getAreaMax(QList<double> areaList)
{
	int index=0;
	double maxArea=areaList.at(0);

	for(int i=1;i<areaList.count();i++)
	{
		double area=areaList.at(i);
		if(area>=maxArea)
		{
			maxArea=area;
			index=i;
		}
	}
	return index;
}
//���������ֶ�
/***
 ˼·����Դͼ�㡢Ŀ��ͼ��Ķ�ӦFeature,Դͼ�㡢Ŀ��ͼ���ֶ��б��ֶε�ͼ�б�Ϊ��������һ��ÿ��Feature��Ӧ���ֶν��и�ֵ
 ���裺
 1.����Ŀ��ͼ����Դͼ����Ҫ�������Կ������ֶ��б�
 2.ͨ���ֶ����Ʋ���Դͼ����Ŀ��ͼ�������Ա��ж�Ӧ���ֶ�����
 3.ͨ��Դͼ���������Ҹ�Դͼ���ĳ��Feature��ĳ���ض��ֶε�ֵ
 4.��Դͼ��ĳ��Feature��Ӧ���ֶ�ֵ��ͨ������ͼ��������ֵ������ͼ��Feature��Ӧ�ֶ�
*/
QgsFeature attributecopyclass::copyattributes(QgsFeature srcFeature,QgsFields *srcFields, QgsFeature targetFeature,QgsFields *targetFields,QMap<QString,QString> *fieldMap)
{
	if(!targetFeature.isValid()||!srcFeature.isValid())return false;
	int fieldSize = srcFeature.fields()->size();
	if(fieldMap->size()>=1)
		fieldSize = fieldMap->size();
	targetFeature.initAttributes(fieldSize);
	/*QList<QString> srcFieldsList = fieldMap->keys();*/
	//1,ѭ���û����õ��ֶ���Ϣ
	for(int i=0;i<fieldMap->count()/2;i++)
	{
		//�����ֶ�ѡ����Ҫ���и�ֵ���ֶ�ʱ���ֶ����ƿ���������ͬ���̽���Ҫ���и�ֵ������ͼ��Ķ�Ӧ�����ֶΣ����������Ƿ���ͬ��
		//�������ֶ�ӳ���б��У�Ŀ��ͼ���ֶ���ǰ��Դͼ���ֶ��ں󣬹�ȡֵʱ�ֱ�����ż��Ϊ����
		QString targetFld = fieldMap->value(QString::number(2*i+1));
		QString srcField=fieldMap->value(QString::number(2*i));
		if(targetFld.isNull()||targetFld.isEmpty())continue;
		//��ȡ�ֶ������Ϣ
		int srcFldIndx ;
		int targetFldIndx;
		QgsField targetField;
		QVariant val;
		
		if(targetFld=="mian_ji"||targetFld=="MIAN_JI")
		{
			//��ȡ�ֶ������Ϣ
			srcFldIndx = srcFields->fieldNameIndex(srcField);
			targetField = targetFields->field(targetFld);
			targetFldIndx = targetFields->fieldNameIndex(targetField.name());
			//����Ŀ���������͹���ֵ
			//�������
			val =QString::number(targetFeature.geometry()->area()/10000,'f',1);
		
			//��ԭʼֵ���ֶ������������ø�Ŀ��Ҫ��ͼ��
			targetFeature.setAttribute(targetFldIndx,val);
		}
		else
		{
			
			//��ȡ�ֶ������Ϣ
			srcFldIndx = srcFields->fieldNameIndex(srcField);
			targetField = targetFields->field(targetFld);
			targetFldIndx = targetFields->fieldNameIndex(targetField.name());
			if(srcField.isNull()||srcField.isEmpty())
			{
				targetFeature.setAttribute(targetFldIndx,"");
			}
			else
			{
				//����Ŀ���������͹���ֵ
				//ȡ��ԭʼ�û����õ�ֵ
				val = srcFeature.attribute(srcFldIndx);
				//��ԭʼֵ���ֶ������������ø�Ŀ��Ҫ��ͼ��
				targetFeature.setAttribute(targetFldIndx,val);
			}
		}
		
	}
	return targetFeature;
}
void attributecopyclass::updateFeature(QgsFeature &feature,QgsVectorLayer *mLayer)
{
	QgsGeometryMap geometryMap;
	geometryMap.insert( feature.id(), QgsGeometry( feature.geometry()->geometry()->clone() ) );
	QgsChangedAttributesMap changedAttributesMap;
	QgsAttributeMap attribMap;
	for ( int i = 0, n = feature.attributes().size(); i < n; ++i )
	{
		attribMap.insert( i, feature.attributes().at( i ) );
	}
	changedAttributesMap.insert( feature.id(), attribMap );
	mLayer->updateFields();
	mLayer->updateFeature(feature);
	mLayer->updateExtents();
	mLayerMutex.lock();
	//mFeatureCache.remove( feature.id() ); // Remove to force reload on next get()
	/*mLayer->dataProvider()->changeGeometryValues( geometryMap );
	mLayer->dataProvider()->changeAttributeValues( changedAttributesMap );*/

	mLayerMutex.unlock();
	mIndexMutex.lock();
	mIndex.deleteFeature( feature );
	mIndex.insertFeature( feature );
	mIndexMutex.unlock();
}

attributecopyclass::~attributecopyclass(void)
{
}
