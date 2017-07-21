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
//两图层间属性拷贝
void attributecopyclass::copyLyrToLyr(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap,int opType)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","图层必须处于可编辑状态！");
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
		//QMessageBox::about(nullptr,"Error","请选中用于进行拷贝的源图层小班！");
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

				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//两图层的面相交区域的面积
				//double targetArea=targetFeature.geometry()->area();//更新图层某个面的面积
				if(intersectArea>0.1)//如果比例大于设定的阈值则进行赋值
				{
					targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//从源图层Feature拷贝字段属性到目标图层Feature
					this->updateFeature(targetFeature,targetLayer);//Feature更新保存		
					
				}

				///*图层属性表中mian_ji单位是公顷，1公顷=10000平方米*/
				//double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//两图层的面相交区域的面积
				////double targetArea=targetFeature.geometry()->area();//更新图层某个面的面积
				//double srcArea=srcFeature.geometry()->area();//源图层面的面积
				//double coverageRate=(intersectArea/srcArea)*100;//相交区域面积占源图层对应面面积的比例

				/*areaList.append(intersectArea);
				srcFList.append(srcFeature);
				targetFList.append(targetFeature);*/
			}
	    
		}
	//	int index=getAreaMax(areaList);//获取相交面积最大的面索引值
		//srcFeature=srcFList.at(index);//获取相交面积最大的面
	//	targetFeature=targetFList.at(index);
		
	}
	QMessageBox::about(nullptr,QObject::tr("提示"),QObject::tr("拷贝完成！"));
}
//裁剪模式拷贝
void attributecopyclass::clipCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","图层必须处于可编辑状态！");
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
				///*图层属性表中mian_ji单位是公顷，1公顷=10000平方米*/
				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//两图层的面相交区域的面积
				areaList.append(intersectArea);
			}
		}
		int index=getAreaMax(areaList);//获取相交面积最大的面索引值
		srcFeature=srcFList.at(index);//获取相交面积最大的面
		targetFeature=targetFList.at(index);
		targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//从源图层Feature拷贝字段属性到目标图层Feature
		this->updateFeature(targetFeature,targetLayer);//Feature更新保存
	}
	QMessageBox::about(nullptr,QObject::tr("提示"),QObject::tr("拷贝完成！"));
}
//分割模式拷贝
void attributecopyclass::splitCopy(QgsVectorLayer *srcLayer,QgsVectorLayer *targetLayer,QMap<QString,QString> *fieldMap)
{
	if(!targetLayer->isEditable())
	{
		QMessageBox::about(nullptr,"Error","图层必须处于可编辑状态！");
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
				double intersectArea=srcFeature.geometry()->intersection(targetFeature.geometry())->area();//两图层的面相交区域的面积
				//double targetArea=targetFeature.geometry()->area();//更新图层某个面的面积
				if(intersectArea>0.1)//如果比例大于设定的阈值则进行赋值
				{
					targetFeature=this->copyattributes(srcFeature,srcFields,targetFeature,targetFields,fieldMap);//从源图层Feature拷贝字段属性到目标图层Feature
					this->updateFeature(targetFeature,targetLayer);//Feature更新保存		
					
				}
			}
	    
		}
		
	}
	QMessageBox::about(nullptr,QObject::tr("提示"),QObject::tr("拷贝完成！"));
}

//获取所相交面积最大值对应的索引
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
//拷贝属性字段
/***
 思路：以源图层、目标图层的对应Feature,源图层、目标图层字段列表及字段地图列表为参数，逐一对每个Feature相应的字段进行赋值
 步骤：
 1.遍历目标图层与源图层所要进行属性拷贝的字段列表
 2.通过字段名称查找源图层与目标图层中属性表中对应的字段索引
 3.通过源图层索引查找该源图层的某个Feature中某个特定字段的值
 4.将源图层某个Feature对应的字段值，通过更新图层索引赋值给更新图层Feature对应字段
*/
QgsFeature attributecopyclass::copyattributes(QgsFeature srcFeature,QgsFields *srcFields, QgsFeature targetFeature,QgsFields *targetFields,QMap<QString,QString> *fieldMap)
{
	if(!targetFeature.isValid()||!srcFeature.isValid())return false;
	int fieldSize = srcFeature.fields()->size();
	if(fieldMap->size()>=1)
		fieldSize = fieldMap->size();
	targetFeature.initAttributes(fieldSize);
	/*QList<QString> srcFieldsList = fieldMap->keys();*/
	//1,循环用户配置的字段信息
	for(int i=0;i<fieldMap->count()/2;i++)
	{
		//由于手动选择所要进行赋值的字段时，字段名称可能有所不同，固将所要进行赋值的两个图层的对应两个字段（无论名称是否相同）
		//都放入字段映射列表中，目标图层字段在前，源图层字段在后，故取值时分别以奇偶数为索引
		QString targetFld = fieldMap->value(QString::number(2*i+1));
		QString srcField=fieldMap->value(QString::number(2*i));
		if(targetFld.isNull()||targetFld.isEmpty())continue;
		//获取字段序号信息
		int srcFldIndx ;
		int targetFldIndx;
		QgsField targetField;
		QVariant val;
		
		if(targetFld=="mian_ji"||targetFld=="MIAN_JI")
		{
			//获取字段序号信息
			srcFldIndx = srcFields->fieldNameIndex(srcField);
			targetField = targetFields->field(targetFld);
			targetFldIndx = targetFields->fieldNameIndex(targetField.name());
			//根据目标数据类型构建值
			//重算面积
			val =QString::number(targetFeature.geometry()->area()/10000,'f',1);
		
			//将原始值按字段类型重新设置给目标要素图层
			targetFeature.setAttribute(targetFldIndx,val);
		}
		else
		{
			
			//获取字段序号信息
			srcFldIndx = srcFields->fieldNameIndex(srcField);
			targetField = targetFields->field(targetFld);
			targetFldIndx = targetFields->fieldNameIndex(targetField.name());
			if(srcField.isNull()||srcField.isEmpty())
			{
				targetFeature.setAttribute(targetFldIndx,"");
			}
			else
			{
				//根据目标数据类型构建值
				//取出原始用户设置的值
				val = srcFeature.attribute(srcFldIndx);
				//将原始值按字段类型重新设置给目标要素图层
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
