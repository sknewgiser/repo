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
QgsFields *srcFields;//Դͼ���ֶμ�
QgsFields *updateFields;//����ͼ���ֶμ�
QgsVectorLayer *srcLyr;//Դͼ��
QgsVectorLayer *updateLyr;//���£�Ŀ�꣩ͼ��
QStringList sameFields;//��¼Դͼ������£�Ŀ�꣩ͼ����ͬ���ֶ�
QStringList selectedFields;//��¼��ѡ����ֶ�
QTimer *timer;
QButtonGroup *btnGroupData;
int currentRow;
int proIndex=0;//����Feature�ĵ�ǰ���
int total=0;//��¼Feature������
int startIndex=0;//ÿ��ѭ��progressbar����ʼֵ
int flag=-1;//������������ֶ�ƥ��ʱ����ӻ�ɾ����ϵʱ������״̬
int copyMode=0;//���Կ���ģʽ��mode=0����ͼ��俽��ģʽ��mode>=1;����ͼ���ڿ���ģʽ,mode=1Ϊ�ָ�;mode=2Ϊ�ü�
int srcSum=0;//Դͼ��Feature����
int updSum=0;//Ŀ��ͼ��Feature����	
int intersectCount=0;//��intersectCount>1ʱ��Ϊ�ü���=1��Ϊ�ָ�
QgsVectorDataProvider *vectorProvider;
QString currentValue="";
attributecopy::attributecopy( QWidget* parent, Qt::WindowFlags fl )
    : QDialog( parent, fl )
{
	//���������������
	//���������
	/* QTextCodec *codec = QTextCodec::codecForName("GB2312");  
  
    QTextCodec::setCodecForLocale(codec);  
    QTextCodec::setCodecForCStrings(codec);  
    QTextCodec::setCodecForTr(codec);*/
	
	//����
	QTextCodec *localCode = QTextCodec::codecForLocale();
	QTextCodec::setCodecForLocale(localCode);
	QTextCodec::setCodecForCStrings(localCode);
	QTextCodec::setCodecForTr(localCode);
	setupUi( this );
	//setFixedSize(757,570);//�̶������С
	this->sure_btn->setEnabled(false);
	GetLayersName();//��ȡͼ������
	initUiEvents();//�����ؼ����źź�����ۺ���������
	
	flag=1;
	
}
//�����ؼ����źź�����ۺ���֮�������
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
		updateFields=new QgsFields(vectorProvider->fields());//��ʼʱ������Դͼ�㻹�Ǹ���ͼ���ʼ��ΪԴͼ���ֶ�ʵ��
		srcSum=vectorProvider->featureCount();
		updSum=updateLyr->allFeatureIds().count();	
		copyMode=1;
		attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//ȡ�����Ա�˫������
		
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
		updateFields=new QgsFields(vectorProvider->fields());//��ʼʱ������Դͼ�㻹�Ǹ���ͼ���ʼ��ΪԴͼ���ֶ�ʵ��
		srcSum=vectorProvider->featureCount();
		updSum=updateLyr->allFeatureIds().count();	
		copyMode=2;
		attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//ȡ�����Ա�˫������
	}
	
}
void attributecopy::initSource()
{
		//srcIter=srcLyr->getFeatures();
	srcSum=vectorProvider->featureCount();
	updSum=updateLyr->allFeatureIds().count();	
	//int sum=srcSum*updSum;//��ͼ��������
	//int dCount=updSum-srcSum;//Դͼ������ͼ��֮���С����֮��
}
//��ȡͼ�����ƣ���ʼ��srcLyrList��updateLyrList
void attributecopy::GetLayersName()
{
	QList<QgsMapCanvasLayer> myLayerSet;//����ͼ���б�
	QgsMapLayerRegistry *pMapLayer;//ͼ�������,δʵ����
	pMapLayer=QgsMapLayerRegistry::instance();//ʵ����
	//��ȡ��ǰMapCanvas�е�ͼ�㼯
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
		this->updateLyrList->addItem(pLayerName);//��ͼ�����Ƽ����б�
		this->srcLyrList->addItem(pLayerName);//��ͼ�����Ƽ����б�
	}
	initUiLists();//��ʼ��srcFields��updateFields
	//�ڴ���ͼ��֮ǰ����Ҫע�����⣬ʹ�����µĴ���ע������
	//ע��QGIS���
	QString myPluginsDir="..\\plugins";
	QString s=QgsProviderRegistry::instance(myPluginsDir)->libraryDirectory().currentPath();
}
//ͨ��srcLyrList��updateLyrList����ʼ��srcFields��updateFields
/*˼·��
	1.������ǰMapCanvas�����е�ͼ��
	2.���ͼ��������ʸ��ͼ���������һ���жϣ���������������ѭ��
	3.���ͼ��������comboBox�б��е�������ͬ�������ͼ�����ֶ�ʵ���ĳ�ʼ��
	*/
void attributecopy::initUiLists()
{
	QgsMapLayerRegistry *pMapLayer;//ͼ�������,δʵ����
	pMapLayer=QgsMapLayerRegistry::instance();//ʵ����
	//��ȡ��ǰMapCanvas�е�ͼ�㼯
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
			updateFields=new QgsFields(lyr->fields());//��ʼʱ������Դͼ�㻹�Ǹ���ͼ���ʼ��ΪԴͼ���ֶ�ʵ��
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

//�ֹ�ƥ��
void attributecopy::on_hand_match_click()
{
	if(srcFields==nullptr||updateFields==nullptr) return;
	//if(flag==0) 
	//{
	//	//QMessageBox::about(this,"��ʾ��Ϣ","ȫ��ƥ����Զ�ƥ��״̬�£��ֹ�ƥ�䲻���ã�");
	//	QMessageBox::information(this, tr("����"), tr("ȫ��ƥ����Զ�ƥ��״̬�£��ֹ�ƥ�䲻���ã�"), QMessageBox::Ok);  
	//	return;
	//}
	connect(this->attribute_table,SIGNAL(cellClicked(int,int)),this,SLOT(on_table_cellclicked(int,int)));
	int rows=this->attribute_table->rowCount();
	this->attribute_table->insertRow(rows);//�ڱ�������Ļ����ϲ���һ��
	QTableWidgetItem *check=new QTableWidgetItem();
	check->setCheckState(Qt::Unchecked);
	this->attribute_table->setItem(rows,0,check);
    flag=1;
}
//�Զ�ƥ��
void attributecopy::on_auto_match_click()
{
	flag=0;
	if(srcFields==nullptr||updateFields==nullptr) return;
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	attribute_table->disconnect(SIGNAL(cellClicked(int,int)));
	this->attribute_table->setRowCount(0);
	this->attribute_table->clearContents();//��ձ��
	int rowIndex=0;
	int rows=this->attribute_table->rowCount();
	//ͨ������srcFields�ֶ���updateFields�ó�����ͼ�����ͬ�����ֶ�
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
				///*ʹcheckBox�ڵ�Ԫ���о�����ʾ*/
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
//ȫ��ƥ��
void attributecopy::on_all_match_click()
{
	flag=0;
	if(srcFields==nullptr||updateFields==nullptr) return;
//	attribute_table->disconnect(SIGNAL(cellClicked(int,int)));//ȡ�����Ա�˫������
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	if(!sameFields.isEmpty())
		sameFields.clear();
	this->attribute_table->setRowCount(0);
	this->attribute_table->clearContents();//��ձ��
	int rowIndex=0;
	int rows=this->attribute_table->rowCount();
	
	QList<QgsField> srcList=srcFields->toList();//��¼Դͼ���ֶμ�
	QList<QgsField> updList=updateFields->toList();//��¼����ͼ���ֶμ�
	Q_FOREACH(QgsField srcField,srcFields->toList())
	{
		Q_FOREACH(QgsField updateField,updateFields->toList())
		{
			//������ͼ���ֶ���ͬ�����ֶ���Ϊ��Ԫ��ǰֵ��ѹ����ͬ�ֶμ���
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
	//�ų������ֶ��б��У�����ͬ�ֶμ�����ͬ���ֶΣ�ȡ��ʣ���ֶ�
	for(int i=0;i<sameFields.count();i++)
	{
		for(int k=0;k<updList.count();k++)
		{
			if(sameFields.at(i)==updList.at(k).name())//�������Ļ���������at������������takeAt()��takeAt(i)��ɾ�������е�Ԫ��i�ĳ���
			{
				updList.removeAt(k);
				break;
			}
		}
	}
	//������ͼ������Դͼ�㲻��ͬ���ֶ���ʾ����
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
//ȫѡ
void attributecopy::on_select_all_click()
{
	flag=0;
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	int rows=this->attribute_table->rowCount();
	for(int i=0;i<rows;i++)
	{
		this->attribute_table->item(i,0)->setCheckState(Qt::Checked);//�Զ�����attribute_table��checkChanged����������ѡ�е�һ���е��ֶμ���ѡ���б���
	}
}
//ɾ����ϵ
void attributecopy::on_delete_relation_click()
{
	//
	if(selectedFields.isEmpty())
		return;
	int rowNum=this->attribute_table->rowCount();
	//������������������Ǳ�ѡ��״̬����ȡ��ѡ��
	for(int i=0;i<rowNum;i++)
	{
		if(this->attribute_table->item(i,0)->checkState()==Qt::Checked)
		{
			this->attribute_table->item(i,0)->setCheckState(Qt::Unchecked);//����ȡ��ѡ��֮����Զ�����attribute_table��checkedChanged�������Ӷ�����Ӧ�ֶδ�ѡ���ֶ��б���ɾ��
		}
	}
}
//ȷ����������
void attributecopy::on_sure_click()
{
	try
	{
		if(!updateLyr->isEditable())
		{
			QMessageBox::about(nullptr,"Error","ͼ����봦�ڿɱ༭״̬��");
			return;
		}
		//���û��ѡ���κ��ֶ��򷵻ؿ�
		if(selectedFields.isEmpty())
			return;
		QMap<QString,QString> *fieldMap=new QMap<QString,QString>();//�����ֶ�ӳ���б�
		for(int i=0;i<selectedFields.count();i++)
		{
			fieldMap->insert(QString::number(i),selectedFields.value(i));
			//�����ֶ�ѡ����Ҫ���и�ֵ���ֶ�ʱ���ֶ����ƿ���������ͬ���̽���Ҫ���и�ֵ������ͼ��Ķ�Ӧ�����ֶΣ����������Ƿ���ͬ��
			//�������ֶ�ӳ���б��С�
		}
		attributecopyclass copy;
		switch(copyMode)
		{
			//case 0 :	//ͼ�����������
			//	//copyLyrToLyr(srcIter,targetIter,fieldMap);
			//	copy.copyLyrToLyr(srcLyr,updateLyr,fieldMap,copyMode,this->numSpare->value());
			//	break;
			case 1 :   //ͼ���ü�����
				//splitCopy(srcIter,targetIter,fieldMap);
				//copy.splitCopy(srcLyr,updateLyr,fieldMap);
				copy.copyLyrToLyr(srcLyr,updateLyr,fieldMap,copyMode);
				break;
			case 2 :   //ͼ���ָ��
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
		QMessageBox::warning(this,QObject::tr("CRS Exception"),QObject::tr("����ʧ�ܣ�"));
	}
	
	
}
//�رյ�ǰ���ܽ���
void attributecopy::on_close()
{
	if(!selectedFields.isEmpty())
		selectedFields.clear();
	this->close();
}


//����ComboBox��activated�źź�����Ӧ�Ĳۺ���
void attributecopy::on_srcComBox_activated(const QString &text)
{
	//��ȡ��ǰMapCanvas��ͼ��
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
			//��ͼ��������Ŀ��ͼ�㣬���ȡĿ��ͼ�������ֶμ���
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
//����ComboBox��activated�źź�����Ӧ�Ĳۺ���
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
//����QTableWidget��cellClicked(int,int)�źź�����Ӧ�Ĳۺ��������������ĵ�Ԫ��ʱ�����¼�
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
	
	/*��flag=0ʱ�������Զ�ƥ���ȫ��ƥ��
	  ��flag>=1ʱ��˵���Ѵ����ֹ�ƥ�䣬�ڱ����ѡ��ĳ�ֶκ�flag��ֵΪ3
	  ��flag=3ʱ��˵������ѡ���ֶμ���selectedFields�ֶ��б���ɽ�����ѡ���ֶε�ɾ��
	  ��flag=2ʱ��˵���Ѿ�����tabele_cellClicked�����������ڴ�״̬�£�flagʼ��Ϊ2
	*/
	if(flag==1||flag==3)
		flag=2;

}
//�������д���������comboBox��activated�źź�����Ӧ�Ĳۺ�������comboBox��value�����仯ʱ�����ú���
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
//����QTableWidget��cellChanged(int,int)�źź�����Ӧ�Ĳۺ����������Ԫ��ֵ�����仯ʱ�����ú���
 
/*��flag=0ʱ�������Զ�ƥ���ȫ��ƥ����ֶ�ѡ���ɾ��
	  ��flag>=1ʱ��˵���Ѵ����ֹ�ƥ�䣬�ڱ����ѡ��ĳ�ֶκ�flag��ֵΪ3
	  ��flag=3ʱ��˵������ѡ���ֶμ���selectedFields�ֶ��б���ɽ�����ѡ���ֶε�ɾ��
	  ��flag=2ʱ��˵���Ѿ�����tabele_cellClicked�����������ڴ�״̬�£�flagʼ��Ϊ2�����ܽ����ֶε�ɾ��
	*/
void attributecopy::on_checkbox_changed(int row,int column)
{
	QList<QString> list;
	if(flag>=1)
	{
		//���checkBoxΪѡ��״̬���򽫶�Ӧ�ֶμ�¼��ѡ���ֶμ�����
		if(this->attribute_table->item(row,0)->checkState()==Qt::Checked)
		{
			if(attribute_table->item(row,2)==nullptr)
				return;
			QString srcFieldName=attribute_table->item(row,2)->text().split('(').at(0);
			//if(attribute_table->item(row,1)==nullptr)
			//{
			//	//���Դͼ���������ֶΣ���Ŀ��ͼ����û�и��ֶΣ�����Ŀ��ͼ�����½����ֶ�
			//	QList<QgsField> newFieldList; 
			//	QgsField field=srcFields->field(srcFieldName);
			//	newFieldList.push_back(field);
			//	QgsVectorDataProvider *vectorProvider=updateLyr->dataProvider();
			//	vectorProvider->addAttributes(newFieldList);//�����ֶ���ӽ���updateLyrͼ�����Ա���
			//	updateFields=new QgsFields(vectorProvider->fields());//���³�ʼ��updateFields
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
			flag=3;//��flagΪ3ʱ��˵�����������ѡ�е��ֶΣ����Խ����ֶε�ɾ��
		}
		//���ȡ����ѡ�У�����ѡ���ֶμ��Ϸǿ�ʱ��ɾ��ȡ�����ֶ�
		if(this->attribute_table->item(row,0)->checkState()==Qt::Unchecked&&!selectedFields.isEmpty())
		{
			if(flag==3)
			{
				if(attribute_table->item(row,1)==nullptr||attribute_table->item(row,2)==nullptr)
					return;
				//��Ԫ�����ֶ���ʾ��ʽΪ��CUN(���ݳ��ȣ�12���ַ�������ʹ��split��ȡ������ǰ����ַ�����
				QString fieldName=attribute_table->item(row,2)->text().split('(').at(0);
				int index=selectedFields.indexOf(fieldName);
				selectedFields.removeAt(index);//������ɾ���ֶ�
				selectedFields.removeAt(index);//��ɾ����һ����ɾ�ֶ�֮����ֶ�
			}
			else
				return;
		}
	}
	if(flag==0)
	{
		//���checkBoxΪѡ��״̬���򽫶�Ӧ�ֶμ�¼��ѡ���ֶμ�����
		if(this->attribute_table->item(row,0)->checkState()==Qt::Checked)
		{
			
			if(attribute_table->item(row,2)==nullptr)
					return;
			QString srcFieldName=attribute_table->item(row,2)->text().split('(').at(0);
	/*		if(attribute_table->item(row,1)==nullptr)//���Դͼ���������ֶΣ���Ŀ��ͼ����û�и��ֶΣ�����Ŀ��ͼ�����½����ֶ�
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
		//���ȡ����ѡ�У�����ѡ���ֶμ��Ϸǿ�ʱ��ɾ��ȡ�����ֶ�
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
	//���û��ѡ��ƥ���ֶΣ����ܽ�����һ�������Կ������������
	if(selectedFields.isEmpty())
		this->sure_btn->setEnabled(false);
	else
		this->sure_btn->setEnabled(true);
}

//����RadioButton��Clicked()�ۺ���
void attributecopy::onRadioClicked()
{
	switch(btnGroupData->checkedId())
	{
		case 0:
			selectedOnly=false;break;//ѡ���������ݽ��п���
		case 1:
			selectedOnly=true;break;//ֻ����ѡ������ݽ��п���
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
//�����򱣴浱ǰ����Ϊxml�ļ�
void attributecopy::onAddSet()
{
	if(this->attribute_table->rowCount()==0)
		return;
	if(this->inputTagName->text().isEmpty())
	{
		QMessageBox::about(this,"Error","���Ʋ���Ϊ�գ���ʽӢ����ѣ�");
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
		QMessageBox::information(this,"��ʾ","�ļ�����ɹ���");
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
 //��updateLyrList���ڿɱ༭״̬�£�EditTextһ���仯�ͻᴥ���ú���
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
//��srcLyrList���ڿɱ༭״̬�£�EditTextһ���仯�ͻᴥ���ú���
void attributecopy::on_srcComBox_EditTextChanged(const QString &text)
{	  
	//��ȡ��ǰMapCanvas��ͼ��
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
			//��ͼ��������Ŀ��ͼ�㣬���ȡĿ��ͼ�������ֶμ���
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
		this->attribute_table->clearContents();//��ձ��
		QDir dir;
		QString filePath=dir.currentPath()+"/xml/config.xml";
		readXML read;
		QList<QPair<QString,QStringList>> catalogList=read.readXml(filePath,text);

		if(catalogList.isEmpty())
			return;
		int row=this->attribute_table->rowCount();
		int rowsNum=catalogList.at(2).second.count();
		attribute_table->setRowCount(row+rowsNum);
		//����һ��ΪcheckBox

		for(int i=0;i<catalogList.at(2).second.count();i++)
		{
			QTableWidgetItem *check=new QTableWidgetItem();
			check->setCheckState(Qt::Unchecked);
			attribute_table->setItem(i,0,check);
		}
		////���ڶ���ΪQComboBox
		for(int i=0;i<catalogList.at(2).second.count();i++)
		{
			QString value=catalogList.at(2).second.at(i);
			QTableWidgetItem *updItem=new QTableWidgetItem(value);
			attribute_table->setItem(i,1,updItem);
		}
		//��������ΪQComboBox
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
		//QMessageBox::about(this,"Info","����ɹ���");
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