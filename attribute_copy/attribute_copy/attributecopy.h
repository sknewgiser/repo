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
#ifndef attributecopy_H
#define  attributecopy_H

#include <QDialog>
#include <ui_attributecopy.h>
#include <qgsmapcanvas.h>
#include <qtablewidget.h>
#include <qgsfield.h>
#include <qgisinterface.h>
#include <qmutex.h>
#include "qgsspatialindex.h"
/**
@author Tim Sutton
*/
class  attributecopy : public QDialog, private Ui::AttributeCopyForm
{
    Q_OBJECT
  public:
     attributecopy( QWidget* parent = 0, Qt::WindowFlags fl = 0 );
    ~ attributecopy();
	//QgsVectorLayer *mLayer;
	 QMutex mLayerMutex;
    QMutex mIndexMutex;
    QgsSpatialIndex mIndex;
  private:
	QgisInterface *mQGisIface;
    static const int context_id = 0;
	void initUiEvents();
	void GetLayersName();
	void initUiLists();
	void initSource();
	bool selectedOnly;
	void readXml();
	//void splitCopy(QgsFeatureIterator srcIter,QgsFeatureIterator targetIter,QMap<QString,QString> *fieldMap);
	//void clipCopy(QgsFeatureIterator srcIter,QgsFeatureIterator targetIter,QMap<QString,QString> *fieldMap);
	//void copyLyrToLyr(QgsFeatureIterator srcIter,QgsFeatureIterator targetIter,QMap<QString,QString> *fieldMap);
	//int getAreaMax(QList<double> areaList);
	//void copyattribute(QgsFields srcFields,QTableWidget tableField);
	QgsFeature copyattributes(QgsFeature srcFeature,QgsFields *srcFields,QgsFeature targetFeature,QgsFields *targetFields,QMap<QString,QString> *fieldMap);
	void updateFeature( QgsFeature& feature,QgsVectorLayer *mLayer);
  private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonBox_helpRequested();
	void on_hand_match_click();
	void on_auto_match_click();
	void on_all_match_click();
	void on_delete_relation_click();
	//void copyattribute(QgsFields *srcFields,QgsFields *targetFields,QTableWidget *tableField);
	//void on_comBox_currentIndexChanged(int Index);
	void on_srcComBox_activated(const QString &text);
	void on_updateComBox_activated(const QString &text);
	void on_srcComBox_EditTextChanged(const QString &text);
	void on_updateComBox_EditTextChanged(const QString &text);
	void on_table_cellclicked(int row,int column);
	void on_select_all_click();
	void on_sure_click();
	void on_close();
//	void timerDone();
	void on_checkbox_changed(int row,int column);
	void on_src_activated(const QString &text);
	void on_upd_activated(const QString &text);
	void onRadioClicked();
	void onAddSet();

	void onSaveSet();
	void onDeleteSet();
	void on_TabWidget_CurrentChanged(int index);
	//void on_cbxConfigList_EditTextChanged(const QString &text);
	void on_cbxConfigList_activated(const QString &text);
	
};

#endif
