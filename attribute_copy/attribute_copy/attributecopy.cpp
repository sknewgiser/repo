/***************************************************************************
 *   Copyright (C) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "attributecopy.h"
#include "ui_attributecopy.h"
#include "qgscontexthelp.h"
#include <qmessagebox.h>
#include <qmainwindow.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayercache.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayerregistry.h>
#include <qgsproviderregistry.h>
#include <qgspluginlayer.h>
#include <qcheckbox.h>
#include <qgsgeometry.h>
#include <QgsVectorDataProvider.h>
#include <qtextcodec.h>
#include <qbuttongroup.h>
#include <QFileDialog.h>
#include <qtimer.h>
#include <qgsgeos.h>
#include "math.h"
#include "iostream"
#include "qgsspatialitefeatureiterator.h"
#include "attributecopyclass.h"
#include "readXML.h"
#include "SilenEditUtils.h"
//qt includes

//standard includes
QString LayerPath;
QgsFields *srcFields;//源图层字段集
QgsFields *updateFields;//更新图层字段集
QgsVectorLayer *srcLyr;//源图层
QgsVectorLayer *updateLyr;//更新（目标）图层
QStringList sameFields;//记录源图层与更新（目标）图层相同的字段
QStringList selectedFields;//记录被选择的字段
QTimer *timer;
QButtonGroup *btnGroupData;
int currentRow;
int proIndex=0;//设置Feature的当前序号
int total=0;//记录Feature的总数
int startIndex=0;//每次循环progressbar的起始值
int flag=-1;//标记用于区分手动匹配时，添加或删除关系时的三种状态
int copyMode=0;//属性拷贝模式，mode=0则是图层间拷贝模式，mode>=1;则是图层内拷贝模式,mode=1为分割;mode=2为裁剪
int srcSum=0;//源图层Feature总数
int updSum=0;//目标图层Feature总数	
int intersectCount=0;//当intersectCount>1时视为裁剪，=1视为分割
QgsVectorDataProvider *vectorProvider;
QString currentValue="";
attributecopy::attributecopy( QWidget* parent, Qt::WindowFlags fl )
    : QDialog( parent, fl )
{
	//解决中文乱码问题
	//这个不管用
	/* QTextCodec *codec = QTextCodec::codecForName("GB2312");  
  
    QTextCodec::setCodecForLocale(codec);  
    QTextCodec::setCodecForCStrings(codec);  
    QTextCodec::setCodecForTr(codec);*/
	
	//管用
	QTextCodec *localCode = QTextCodec::codecForLocale();
	QTextCodec::setCodecForLocale(localCode);
	QTextCodec::setCodecForCStrings(localCode);
	QTextCodec::setCodecForTr(localCode);
	setupUi( this );
	//setFixedSize(757,570);//固定窗体大小
	this->sure_btn->setEnabled(false);
	GetLayersName();//获取图层名称
	initUiEvents();//建立控件的信号函数与槽函数的连接
	
	flag=1;
	
}
//建立控件的信号函数与槽函数之间的连接
void attributecopy::initUiEvents()
{
	sure_btn->setVisible(false);
	close_btn->setVisible(false);
	this->progressBar->setVisible(false);
	this->valueNum->setVisible(false);

	btnGroupData=new QButtonGroup(this);
	btnGroupData->addButton(this->alldata_radio,0);
	btnGroupData->addButton(this->dataset_radio,1);
	dataset_radio->setChecked(true);
	selectedOnly=false;
	connect(this->hand_match,SIGNAL(clicked()),this,SLOT(on_hand_match_click()));
	connect(this->all_match,SIGNAL(clicked()),this,SLOT(on_all_match_click()));
	connect(this->auto_match,SIGNAL(clicked()),this,SLOT(on_auto_match_click()));
	connect(this->del_relation,SIGNAL(clicked()),this,SLOT(on_delete_relation_click()));
	connect(this->updateLyrList,SIGNAL(activated(const QString &)),this,SLOT(on_updateComBox_activated(const QString &)));
	connect(this->srcLyrList,SIGNAL(activated(const QString &)),this,SLOT(on_srcComBox_activated(const QString &)));
	connect(this->attribute_table,SIGNAL(cellClicked(int,int)),this,SLOT(on_table_cellclicked(int,int)));
	connect(this->select_all,SIGNAL(clicked()),this,SLOT(on_select_all_click()));
	connect(this->sure_btn,SIGNAL(clicked()),this,SLOT(on_sure_click()));
	connect(this->close_btn,SIGNAL(clicked()),this,SLOT(on_close()));
	connect(this->attribute_table,SIGNAL(cellChanged(int,int)),this,SLOT(on_checkbox_changed(int,int)));
	connect(this->alldata_radio,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
	connect(this->dataset_radio,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
	connect(this->addbtn,SIGNAL(clicked()),this,SLOT(onAddSet()));
	connect(this->savebtn,SIGNAL(clicked()),this,SLOT(onSaveSet()));
	connect(this->delbtn,SIGNAL(clicked()),this,SLOT(onDeleteSet()));
	connect(this->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(on_TabWidget_CurrentChanged(int)));
	//connect(this->cbxConfigList,SIGNAL(EditTextChanged(const QString &)),this,SLOT(on_cbxConfigList_EditTextChanged(const QString &text)));
	connect(this->cbxConfigList,SIGNAL(activated(const QString &)),this,SLOT(on_cbxConfigList_activated(const QString &text)));
	
}
void attributecopy::on_TabWidget_CurrentChanged(int index)
{
	if(index==0)
	{
		spareoverride_label->setVisible(false);
		numSpare->setVisible(false);
		label_4->setVisible(false);
		updateLyrList->setEnabled(true);
		srcLyrList->setEnabled(true);
		spareoverride_label->setVisible(true);
		numSpare->setVisible(true);
		label_4->setVisible(true);
		hand_match->setVisible(true);
		auto_match->setVisible(true);
		all_match->setVisible(true);
		dataSetGroup->setVisible(true);
		dataset_radio->setChecked(true);
		sure_btn->setVisible(false);
		close_btn->setVisible(false);
		this->progressBar->setVisible(false);
		this->valueNum->setVisible(false);
		copyMode=0;
		srcFields=new QgsFields(srcLyr->fields());
		updateFields=new QgsFields(updateLyr->fields());
		connect(this->attribute_table,SIGNAL(cellClicked(int,int)),this,SLOT(on_table_cellclicked(int,int)));
	}
	if(index==1)
	{
		/*updateLyrList->setEnabled(false);
		srcLyrList->setEnabled(false);*/
		spareoverride_label->setVisible(false);
		numSpare->setVisible(false);
		label_4->setVisible(false);
		hand_match->setVisible(false);
		auto_match->setVisible(false);
		all_match->setVisible(false);
		dataSetGroup->setVisible(false);
		dataset_radio->setChecked(true);
		sure_btn->setVisible(true);
		close_btn->setVisible(true);
		/*this->progressBar->setVisible(true);
		this->valueNum->setVisible(true);*/
		srcFields=new QgsFields(vectorProvider->fields());
		updateFields=new QgsFields(vectorProvider->fields());//初始时无论是源图层还是更新图层初始化为源图层字段实例
		srcSum=vectorProvider->featureCount();
		updSum=updateLyr->allFeatureIds().count();	
		copyMode=1;
		attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//取消属性表双击函数
		
	}
	if(index==2)
	{
		/*updateLyrList->setEnabled(false);
		srcLyrList->setEnabled(false);*/
		spareoverride_label->setVisible(false);
		numSpare->setVisible(false);
		label_4->setVisible(false);
		hand_match->setVisible(false);
		auto_match->setVisible(false);
		all_match->setVisible(false);
		dataSetGroup->setVisible(false);
		dataset_radio->setChecked(true);
		sure_btn->setVisible(true);
		close_btn->setVisible(true);
		/*this->progressBar->setVisible(true);
		this->valueNum->setVisible(true);*/
		srcFields=new QgsFields(vectorProvider->fields());
		updateFields=new QgsFields(vectorProvider->fields());//初始时无论是源图层还是更新图层初始化为源图层字段实例
		srcSum=vectorProvider->featureCount();
		updSum=updateLyr->allFeatureIds().count();	
		copyMode=2;
		attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//取消属性表双击函数
	}
	
}
void attributecopy::initSource()
{
		//srcIter=srcLyr->getFeatures();
	srcSum=vectorProvider->featureCount();
	updSum=updateLyr->allFeatureIds().count();	
	//int sum=srcSum*updSum;//两图遍历次数
	//int dCount=updSum-srcSum;//源图层与新图层之间的小班数之差
}
//获取图层名称，初始化srcLyrList与updateLyrList
void attributecopy::GetLayersName()
{
	QList<QgsMapCanvasLayer> myLayerSet;//绘制图层列表
	QgsMapLayerRegistry *pMapLayer;//图层管理器,未实例化
	pMapLayer=QgsMapLayerRegistry::instance();//实例化
	//获取当前MapCanvas中的图层集
	QMap <QString, QgsMapLayer*> layers = pMapLayer->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;

	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );

		if ( !lyr )
		{
			continue;
		}
		myLayerSet.append(lyr);
	}

	QString pLayerName="";
	for(int i=0;i<myLayerSet.count();i++)
	{
		pLayerName=myLayerSet.at(i).layer()->name();
		this->updateLyrList->addItem(pLayerName);//将图层名称加入列表
		this->srcLyrList->addItem(pLayerName);//将图层名称加入列表
	}
	initUiLists();//初始化srcFields与updateFields
	//在创建图层之前，需要注册插件库，使用如下的代码注册插件库
	//注册QGIS插件
	QString myPluginsDir="..\\plugins";
	QString s=QgsProviderRegistry::instance(myPluginsDir)->libraryDirectory().currentPath();
}
//通过srcLyrList与updateLyrList来初始化srcFields与updateFields
/*思路：
	1.遍历当前MapCanvas中所有的图层
	2.如果图层类型是矢量图层则进行下一步判断，否则跳出，继续循环
	3.如果图层名称与comboBox列表中的名称相同，则进行图层与字段实例的初始化
	*/
void attributecopy::initUiLists()
{
	QgsMapLayerRegistry *pMapLayer;//图层管理器,未实例化
	pMapLayer=QgsMapLayerRegistry::instance();//实例化
	//获取当前MapCanvas中的图层集
	QMap <QString, QgsMapLayer*> layers = pMapLayer->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;
	
	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );
		if(lyr->name()==srcLyrList->currentText())
		{
			srcLyr=lyr;
			updateLyr=lyr;
			
			vectorProvider=lyr->dataProvider();
			srcFields=new QgsFields(lyr->fields());
			updateFields=new QgsFields(lyr->fields());//初始时无论是源图层还是更新图层初始化为源图层字段实例
		}
		if ( !lyr )
		{
			continue;
		}
	}
	QDir dir;
	QString filePath=dir.currentPath()+"/xml/config.xml";
	readXML read;
	QStringList nodesTagNameList=read.getNodesTagName(filePath);
    this->cbxConfigList->addItems(nodesTagNameList);
}

//手工匹配
void attributecopy::on_hand_match_click()
{
	if(srcFields==nullptr||updateFields==nullptr) return;
	//if(flag==0) 
	//{
	//	//QMessageBox::about(this,"提示信息","全部匹配或自动匹配状态下，手工匹配不可用！");
	//	QMessageBox::information(this, tr("警告"), tr("全部匹配或自动匹配状态下，手工匹配不可用！"), QMessageBox::Ok);  
	//	return;
	//}
	connect(this->attribute_table,SIGNAL(cellClicked(int,int)),this,SLOT(on_table_cellclicked(int,int)));
	int rows=this->attribute_table->rowCount();
	this->attribute_table->insertRow(rows);//在表格行数的基础上插入一行
	QTableWidgetItem *check=new QTableWidgetItem();
	check->setCheckState(Qt::Unchecked);
	this->attribute_table->setItem(rows,0,check);
    flag=1;
}
//自动匹配
void attributecopy::on_auto_match_click()
{
	flag=0;
	if(srcFields==nullptr||updateFields==nullptr) return;
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	attribute_table->disconnect(SIGNAL(cellClicked(int,int)));
	this->attribute_table->setRowCount(0);
	this->attribute_table->clearContents();//清空表格
	int rowIndex=0;
	int rows=this->attribute_table->rowCount();
	//通过遍历srcFields字段与updateFields得出两个图层的相同属性字段
	Q_FOREACH(QgsField srcField,srcFields->toList())
	{
		Q_FOREACH(QgsField updateField,updateFields->toList())
		{
			if(srcField.name()==updateField.name())
			{
				rowIndex++;
				attribute_table->setRowCount(rows+rowIndex);
				QTableWidgetItem *check=new QTableWidgetItem();
				check->setCheckState(Qt::Unchecked);
				attribute_table->setItem(rowIndex-1,0,check);
				//checkBox=new QCheckBox();
				///*使checkBox在单元格中居中显示*/
				//QHBoxLayout *hLayout=new QHBoxLayout();
				//QWidget *widget=new QWidget(attribute_table);
				//hLayout->addWidget(checkBox);
				//hLayout->setMargin(0);
				//hLayout->setAlignment(checkBox,Qt::AlignCenter);
				//widget->setLayout(hLayout);
				//attribute_table->setCellWidget(rowIndex-1,0,widget);
				QString srcFTName=srcField.typeName();
				QString updFTName=updateField.typeName();
	
				QTableWidgetItem *srcItem=new QTableWidgetItem(srcField.name()+"("+srcFTName+")");
				QTableWidgetItem *updateItem=new QTableWidgetItem(updateField.name()+"("+updFTName+")");
	
				attribute_table->setItem(rowIndex-1,2,srcItem);
				attribute_table->setItem(rowIndex-1,1,updateItem);
				break;
			}
		}
	}
}
//全部匹配
void attributecopy::on_all_match_click()
{
	flag=0;
	if(srcFields==nullptr||updateFields==nullptr) return;
//	attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//取消属性表双击函数
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	if(!sameFields.isEmpty())
		sameFields.clear();
	this->attribute_table->setRowCount(0);
	this->attribute_table->clearContents();//清空表格
	int rowIndex=0;
	int rows=this->attribute_table->rowCount();
	
	QList<QgsField> srcList=srcFields->toList();//记录源图层字段集
	QList<QgsField> updList=updateFields->toList();//记录更新图层字段集
	Q_FOREACH(QgsField srcField,srcFields->toList())
	{
		Q_FOREACH(QgsField updateField,updateFields->toList())
		{
			//若两个图层字段相同，则字段设为单元格当前值并压入相同字段集合
			if(srcField.name()==updateField.name())
			{
				rowIndex++;
				attribute_table->setRowCount(rows+rowIndex);
				QTableWidgetItem *check=new QTableWidgetItem();
				check->setCheckState(Qt::Unchecked);
				attribute_table->setItem(rowIndex-1,0,check);
				QString srcFTName=srcField.typeName();
				QString updFTName=updateField.typeName();

				QTableWidgetItem *srcItem=new QTableWidgetItem(srcField.name()+"("+srcFTName+")");
				QTableWidgetItem *updateItem=new QTableWidgetItem(updateField.name()+"("+updFTName+")");
				attribute_table->setItem(rowIndex-1,2,srcItem);
				attribute_table->setItem(rowIndex-1,1,updateItem);
				sameFields.append(srcField.name());
			}
		}
	}
	//排除更新字段列表中，与相同字段集的相同的字段，取得剩余字段
	for(int i=0;i<sameFields.count();i++)
	{
		for(int k=0;k<updList.count();k++)
		{
			if(sameFields.at(i)==updList.at(k).name())//输出链表的话，可以用at（），而不是takeAt()，takeAt(i)是删除链表中的元素i的长度
			{
				updList.removeAt(k);
				break;
			}
		}
	}
	//将更新图层中与源图层不相同的字段显示出来
	if(updList.count()!=0)
	{
		for(int n=0;n<updList.count();n++)
		{
			int rowNum2=attribute_table->rowCount();
			attribute_table->setRowCount(rowNum2+1);
			QTableWidgetItem *check=new QTableWidgetItem();
			check->setCheckState(Qt::Unchecked);
			attribute_table->setItem(rowNum2,0,check);

			QString updFTName=updList.at(n).typeName();
			
			QTableWidgetItem *updateItem=new QTableWidgetItem(updList.at(n).name()+"("+updFTName+")");
			attribute_table->setItem(rowNum2,1,updateItem);
		}
	}
}
//全选
void attributecopy::on_select_all_click()
{
	flag=0;
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	int rows=this->attribute_table->rowCount();
	for(int i=0;i<rows;i++)
	{
		this->attribute_table->item(i,0)->setCheckState(Qt::Checked);//自动触发attribute_table的checkChanged函数，将该选中的一行中的字段加入选中列表中
	}
}
//删除关系
void attributecopy::on_delete_relation_click()
{
	//
	if(selectedFields.isEmpty())
		return;
	int rowNum=this->attribute_table->rowCount();
	//遍历所有行如果该行是被选中状态，则取消选中
	for(int i=0;i<rowNum;i++)
	{
		if(this->attribute_table->item(i,0)->checkState()==Qt::Checked)
		{
			this->attribute_table->item(i,0)->setCheckState(Qt::Unchecked);//设置取消选中之后会自动触发attribute_table的checkedChanged函数，从而将对应字段从选中字段列表中删除
		}
	}
}
//确定拷贝属性
void attributecopy::on_sure_click()
{
	try
	{
		if(!updateLyr->isEditable())
		{
			QMessageBox::about(nullptr,"Error","图层必须处于可编辑状态！");
			return;
		}
		//如果没有选择任何字段则返回空
		if(selectedFields.isEmpty())
			return;
		QMap<QString,QString> *fieldMap=new QMap<QString,QString>();//创建字段映射列表
		for(int i=0;i<selectedFields.count();i++)
		{
			fieldMap->insert(QString::number(i),selectedFields.value(i));
			//由于手动选择所要进行赋值的字段时，字段名称可能有所不同，固将所要进行赋值的两个图层的对应两个字段（无论名称是否相同）
			//都放入字段映射列表中。
		}
		attributecopyclass copy;
		switch(copyMode)
		{
			//case 0 :	//图层间新增拷贝
			//	//copyLyrToLyr(srcIter,targetIter,fieldMap);
			//	copy.copyLyrToLyr(srcLyr,updateLyr,fieldMap,copyMode,this->numSpare->value());
			//	break;
			case 1 :   //图层间裁剪拷贝
				//splitCopy(srcIter,targetIter,fieldMap);
				//copy.splitCopy(srcLyr,updateLyr,fieldMap);
				copy.copyLyrToLyr(srcLyr,updateLyr,fieldMap,copyMode);
				break;
			case 2 :   //图层间分割拷贝
				//clipCopy(srcIter,targetIter,fieldMap);
				//copy.clipCopy(srcLyr,updateLyr,fieldMap);
				copy.copyLyrToLyr(srcLyr,updateLyr,fieldMap,copyMode);
				break;
		}	
		if(!selectedFields.isEmpty())
			selectedFields.clear();
		this->sure_btn->setEnabled(false);
	}
	catch(QgsCsException &cse){
		Q_UNUSED(cse);
		QMessageBox::warning(this,QObject::tr("CRS Exception"),QObject::tr("拷贝失败！"));
	}
	
	
}
//关闭当前功能界面
void attributecopy::on_close()
{
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	this->close();
}


//定义ComboBox的activated信号函数对应的槽函数
void attributecopy::on_srcComBox_activated(const QString &text)
{
	//获取当前MapCanvas中图层
	QMap <QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;
	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );
		if(lyr->name()==text)
		{
			//若图层名称是目标图层，则获取目标图层属性字段集合
			srcFields=new QgsFields(lyr->fields());
			srcLyr=lyr;
			break;
		}
		if ( !lyr )
		{
			continue;
		}
	}
    
}
//定义ComboBox的activated信号函数对应的槽函数
void attributecopy::on_updateComBox_activated(const QString &text)
{
	QMap <QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;
	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );
		if(lyr->name()==text)
		{
			updateFields=new QgsFields(lyr->fields());
			updateLyr=lyr;
			break;
		}
		if ( !lyr )
		{
			continue;
		}
	}
}
//定义QTableWidget的cellClicked(int,int)信号函数对应的槽函数，当单击表格的单元格时触发事件
void attributecopy::on_table_cellclicked(int row,int column)
{

	if(column==0) return;
	if(srcFields->count()==0||updateFields->count()==0) return;
	currentRow=row;
	
	QComboBox *srccomBox=new QComboBox();
//	QComboBox *updatecomBox=new QComboBox();
	srccomBox->setEditable(true);
//	updatecomBox->setEditable(true);
	if(column==2)
		attribute_table->setCellWidget(row,2,srccomBox);
	//attribute_table->setCellWidget(row,1,updatecomBox);
	srccomBox->addItem(nullptr);
	Q_FOREACH(QgsField field,srcFields->toList())
	{
		QString srcFTName=field.typeName();
		srccomBox->addItem(field.name()+"("+srcFTName+")");
	}
	//Q_FOREACH(QgsField field,updateFields->toList())
	//{
	//	QString updFTName=field.typeName();
	
	//	//updatecomBox->addItem(field.name()+"("+updFTName+")");
	//}
	currentValue=this->attribute_table->currentItem()->text();
	srccomBox->setEditText(currentValue);

	//QTableWidgetItem *srcItem=new QTableWidgetItem();
	//QTableWidgetItem *updateItem=new QTableWidgetItem(updatecomBox->currentText());
	//attribute_table->setItem(row,1,updateItem);
//	attribute_table->setItem(row,2,srcItem);
	
	connect(srccomBox,SIGNAL(activated(const QString &)),this,SLOT(on_src_activated(const QString &)));
	
	//connect(updatecomBox,SIGNAL(activated(const QString &)),this,SLOT(on_upd_activated(const QString &)));
	
	/*当flag=0时，进行自动匹配或全部匹配
	  当flag>=1时，说明已触发手工匹配，在表格中选中某字段后，flag赋值为3
	  当flag=3时，说明已有选中字段加入selectedFields字段列表，则可进行已选中字段的删除
	  当flag=2时，说明已经触发tabele_cellClicked函数，并且在此状态下，flag始终为2
	*/
	if(flag==1||flag==3)
		flag=2;

}
//定义表格中创建的两个comboBox的activated信号函数对应的槽函数，当comboBox的value发生变化时触发该函数
void attributecopy::on_src_activated(const QString &text)
{	
	QTableWidgetItem *srcItem=new QTableWidgetItem(text);
	attribute_table->setItem(currentRow,2,srcItem);
}
void attributecopy::on_upd_activated(const QString &text)
{
	/*QTableWidgetItem *updateItem=new QTableWidgetItem(text);
	attribute_table->setItem(currentRow,1,updateItem);*/
}
//定义QTableWidget的cellChanged(int,int)信号函数对应的槽函数，当表格单元格值发生变化时触发该函数
 
/*当flag=0时，进行自动匹配或全部匹配的字段选择或删除
	  当flag>=1时，说明已触发手工匹配，在表格中选中某字段后，flag赋值为3
	  当flag=3时，说明已有选中字段加入selectedFields字段列表，则可进行已选中字段的删除
	  当flag=2时，说明已经触发tabele_cellClicked函数，并且在此状态下，flag始终为2，不能进行字段的删除
	*/
void attributecopy::on_checkbox_changed(int row,int column)
{
	QList<QString> list;
	if(flag>=1)
	{
		//如果checkBox为选中状态，则将对应字段记录到选中字段集合中
		if(this->attribute_table->item(row,0)->checkState()==Qt::Checked)
		{
			if(attribute_table->item(row,2)==nullptr)
				return;
			QString srcFieldName=attribute_table->item(row,2)->text().split('(').at(0);
			//if(attribute_table->item(row,1)==nullptr)
			//{
			//	//如果源图层中有新字段，而目标图层中没有该字段，则在目标图层中新建该字段
			//	QList<QgsField> newFieldList; 
			//	QgsField field=srcFields->field(srcFieldName);
			//	newFieldList.push_back(field);
			//	QgsVectorDataProvider *vectorProvider=updateLyr->dataProvider();
			//	vectorProvider->addAttributes(newFieldList);//将新字段添加进入updateLyr图层属性表中
			//	updateFields=new QgsFields(vectorProvider->fields());//重新初始化updateFields
			//	list.append(srcFieldName);
			//	list.append(srcFieldName);
			//}
			//else{
			QString updFieldName=attribute_table->item(row,1)->text().split('(').at(0);
			list.append(srcFieldName);
			list.append(updFieldName);
			//}
			if(column==3) return;
			selectedFields.append(list);
			flag=3;//当flag为3时则说明表格中已有选中的字段，可以进行字段的删除
		}
		//如果取消了选中，并且选中字段集合非空时，删除取消的字段
		if(this->attribute_table->item(row,0)->checkState()==Qt::Unchecked&&!selectedFields.isEmpty())
		{
			if(flag==3)
			{
				if(attribute_table->item(row,1)==nullptr||attribute_table->item(row,2)==nullptr)
					return;
				//单元格中字段显示形式为“CUN(数据长度：12，字符串）故使用split提取‘（’前后的字符串”
				QString fieldName=attribute_table->item(row,2)->text().split('(').at(0);
				int index=selectedFields.indexOf(fieldName);
				selectedFields.removeAt(index);//按索引删除字段
				selectedFields.removeAt(index);//再删除上一个所删字段之后的字段
			}
			else
				return;
		}
	}
	if(flag==0)
	{
		//如果checkBox为选中状态，则将对应字段记录到选中字段集合中
		if(this->attribute_table->item(row,0)->checkState()==Qt::Checked)
		{
			
			if(attribute_table->item(row,2)==nullptr)
					return;
			QString srcFieldName=attribute_table->item(row,2)->text().split('(').at(0);
	/*		if(attribute_table->item(row,1)==nullptr)//如果源图层中有新字段，而目标图层中没有该字段，则在目标图层中新建该字段
			{
				QList<QgsField> newFieldList; 
				QgsField field=srcFields->field(srcFieldName);
				newFieldList.push_back(field);
				QgsVectorDataProvider *vectorProvider=updateLyr->dataProvider();
				vectorProvider->addAttributes(newFieldList);
				updateFields=new QgsFields(vectorProvider->fields());
				list.append(srcFieldName);
				list.append(srcFieldName);
			}else{*/
			QString updFieldName=attribute_table->item(row,1)->text().split('(').at(0);
			list.append(srcFieldName);
			list.append(updFieldName);
			
			/*}*/
			selectedFields.append(list);
		}
		//如果取消了选中，并且选中字段集合非空时，删除取消的字段
		if(this->attribute_table->item(row,0)->checkState()==Qt::Unchecked&&!selectedFields.isEmpty())
		{
			if(attribute_table->item(row,2)==nullptr)
					return;
			QString fieldName=attribute_table->item(row,2)->text().split('(').at(0);

			int index=selectedFields.indexOf(fieldName);
			selectedFields.removeAt(index);
			selectedFields.removeAt(index);
		}
	}
	//如果没有选择匹配字段，则不能进行下一步的属性拷贝，否则可以
	if(selectedFields.isEmpty())
		this->sure_btn->setEnabled(false);
	else
		this->sure_btn->setEnabled(true);
}

//定义RadioButton的Clicked()槽函数
void attributecopy::onRadioClicked()
{
	switch(btnGroupData->checkedId())
	{
		case 0:
			selectedOnly=false;break;//选择所有数据进行拷贝
		case 1:
			selectedOnly=true;break;//只有所选择的数据进行拷贝
	}
}
void attributecopy::onSaveSet()
{
	if(this->attribute_table->rowCount()==0)
		return;
	QDir dir;
	QString filePath=dir.currentPath()+"/xml/config.xml";
	readXML write;
	write.saveCurrentNode(filePath,attribute_table,this->cbxConfigList->currentText(),srcLyrList->currentText(),updateLyrList->currentText());
	//write.deleteNode(filePath,this->cbxConfigList->currentText());
	//this->cbxConfigList->removeItem(this->cbxConfigList->currentIndex());
}
//导出或保存当前设置为xml文件
void attributecopy::onAddSet()
{
	if(this->attribute_table->rowCount()==0)
		return;
	if(this->inputTagName->text().isEmpty())
	{
		QMessageBox::about(this,"Error","名称不能为空，格式英文最佳！");
		return;
	}
	QDir dir;
	QString filePath=dir.currentPath()+"/xml/config.xml";
	readXML write;
	write.writeXml(filePath,attribute_table,inputTagName->text(),srcLyrList->currentText(),updateLyrList->currentText());
	this->cbxConfigList->addItem(this->inputTagName->text());
	/*if(attribute_table->rowCount()==0)
		return;
	QString myPluginsDir="..\\plugins";
	QString filePath=QgsProviderRegistry::instance(myPluginsDir)->libraryDirectory().currentPath();
	QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"), filePath, tr("(*.xml)"));
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this,"Error: Cannot write file: ",file.errorString());
		return;
	}
 
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("attributeCopy");
	xmlWriter.writeStartElement("VectorLayers");
	xmlWriter.writeAttribute("caption", "the names of the srcLayer and updateLayer");
	xmlWriter.writeTextElement("name", this->updateLyrList->currentText());
	xmlWriter.writeTextElement("name", this->srcLyrList->currentText());
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("selectedState");
	xmlWriter.writeAttribute("caption","select the match fields!");
	for(int k=0;k<this->attribute_table->rowCount();k++)
	{
		if(this->attribute_table->item(k,0)->checkState()==Qt::Checked)
		{
			xmlWriter.writeTextElement("id",QString::number(k));
		}
	}
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("updateFields");
	xmlWriter.writeAttribute("caption", "same fields of updateLayer and srcLayer");
	for(int i=0;i<this->attribute_table->rowCount();i++)
	{
		if(this->attribute_table->item(i,1)==nullptr)
			xmlWriter.writeTextElement("fieldName","");
		else
			xmlWriter.writeTextElement("fieldName",this->attribute_table->item(i,1)->text());
	}
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("srcFields");
	xmlWriter.writeAttribute("caption", "same fields of updateLayer and srcLayer");
	for(int i=0;i<this->attribute_table->rowCount();i++)
	{
		if(this->attribute_table->item(i,2)==nullptr)
			xmlWriter.writeTextElement("fieldName","");
		else
			xmlWriter.writeTextElement("fieldName",this->attribute_table->item(i,2)->text());
	}
	xmlWriter.writeEndElement();
	xmlWriter.writeEndDocument();
	file.close();
	if (file.error()) {
		QMessageBox::warning(this,"Error: Cannot write file: ",file.errorString());
		return;
	}else{
		QMessageBox::information(this,"提示","文件保存成功！");
	}*/
}
void attributecopy::onDeleteSet()
{
	if(this->attribute_table->rowCount()==0)
		return;
	QDir dir;
	QString filePath=dir.currentPath()+"/xml/config.xml";
	readXML write;
	//write.saveCurrentNode(filePath,attribute_table,this->cbxConfigList->currentText(),srcLyrList->currentText(),updateLyrList->currentText());
	write.deleteNode(filePath,this->cbxConfigList->currentText());
	this->cbxConfigList->removeItem(this->cbxConfigList->currentIndex());
	this->attribute_table->setRowCount(0);
	this->attribute_table->clearContents();
}
 //当updateLyrList的在可编辑状态下，EditText一旦变化就会触发该函数
void attributecopy::on_updateComBox_EditTextChanged(const QString &text)
{
	QMap <QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;
	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );
		if(lyr->name()==text)
		{
			updateFields=new QgsFields(lyr->fields());
			updateLyr=lyr;
			break;
		}
		if ( !lyr )
		{
			continue;
		}
	}
}
//当srcLyrList的在可编辑状态下，EditText一旦变化就会触发该函数
void attributecopy::on_srcComBox_EditTextChanged(const QString &text)
{	  
	//获取当前MapCanvas中图层
	QMap <QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
	QMapIterator <QString, QgsMapLayer*> item( layers );
	QgsMapLayer * mapLayer = nullptr;
	QgsVectorLayer * lyr = nullptr;
	while ( item.hasNext() )
	{
		item.next();
		mapLayer = item.value();
		if ( mapLayer->type() != QgsMapLayer::VectorLayer )
		{
			continue;
		}
		lyr = qobject_cast<QgsVectorLayer *>( mapLayer );
		if(lyr->name()==text)
		{
			//若图层名称是目标图层，则获取目标图层属性字段集合
			srcFields=new QgsFields(lyr->fields());
			srcLyr=lyr;
			break;
		}
		if ( !lyr )
		{
			continue;
		}
	}
}

void attributecopy::on_cbxConfigList_activated(const QString &text)
{
	try{
		this->attribute_table->setRowCount(0);
		this->attribute_table->clearContents();//清空表格
		QDir dir;
		QString filePath=dir.currentPath()+"/xml/config.xml";
		readXML read;
		QList<QPair<QString,QStringList>> catalogList=read.readXml(filePath,text);

		if(catalogList.isEmpty())
			return;
		int row=this->attribute_table->rowCount();
		int rowsNum=catalogList.at(2).second.count();
		attribute_table->setRowCount(row+rowsNum);
		//填充第一列为checkBox

		for(int i=0;i<catalogList.at(2).second.count();i++)
		{
			QTableWidgetItem *check=new QTableWidgetItem();
			check->setCheckState(Qt::Unchecked);
			attribute_table->setItem(i,0,check);
		}
		////填充第二列为QComboBox
		for(int i=0;i<catalogList.at(2).second.count();i++)
		{
			QString value=catalogList.at(2).second.at(i);
			QTableWidgetItem *updItem=new QTableWidgetItem(value);
			attribute_table->setItem(i,1,updItem);
		}
		//填充第三列为QComboBox
		for(int j=0;j<catalogList.at(3).second.count();j++)
		{
			QString value=catalogList.at(3).second.at(j);
			QTableWidgetItem *srcItem=new QTableWidgetItem(value);
			attribute_table->setItem(j,2,srcItem);
		}
		for(int s=0;s<catalogList.at(1).second.count();s++)
		{
			int index=catalogList.at(1).second.at(s).toInt();
			attribute_table->item(index,0)->setCheckState(Qt::Checked);
		}
		//QMessageBox::about(this,"Info","导入成功！");
	}
	catch(QString exception)
	{
		QMessageBox::about(this,"Error",exception);
	}
	
}
attributecopy::~attributecopy()
{
}

void attributecopy::on_buttonBox_accepted()
{
  //close the dialog
  accept();
}

void attributecopy::on_buttonBox_rejected()
{
  reject();
}

void attributecopy::on_buttonBox_helpRequested()
{
  QgsContextHelp::run( context_id );
}