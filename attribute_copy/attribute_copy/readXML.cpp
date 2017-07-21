#include "readXML.h"
#include <qfile.h>
#include <qfileinfo.h>
#include <qdom.h>
#include <qfiledialog.h>
#include <qpair.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qxmlstream.h>
#include <QTextCodec.h>
#include <qtablewidget.h>

readXML::readXML(void)
{
}
QStringList readXML::getNodesTagName(QString filePath)
{
	QStringList catalogNameList;
	QDomDocument doc;
	QString fileName=filePath;
	if("" == fileName)
    {
        qDebug()<<"Filename is Null";
        return QStringList();
    }
    QFile file(fileName);
	//QFile file(DirectorOf("xml").absoluteFilePath(fileName));
    if(!file.open(QFile::ReadOnly | QFile::Text))
        qDebug()<<"open file"<<fileName<<"failed, error:"<<file.errorString();
    /*����Dom�ڵ�*/
       QDomDocument    document;
       QString         strError;
       int             errLin = 0, errCol = 0;
       if( !document.setContent(&file, false, &strError, &errLin, &errCol) ) {
           qDebug()<<"parse file failed at line"<<errLin<<",column"<<errCol<<","<<strError;
           return QStringList();
       }
 
       if( document.isNull() ) {
           qDebug()<<"document is null !";
           return QStringList();
       }
 
       QDomElement root = document.documentElement();
       qDebug()<<root.tagName();
 
       QDomElement catalogs = root.firstChildElement();
       if( catalogs.isNull() )
           return QStringList();
       else
	   {
		   while(!catalogs.isNull())
		   {
			   qDebug()<<catalogs.tagName();
			   catalogNameList.append(catalogs.tagName());
			   catalogs=catalogs.nextSiblingElement();
		   }
           return catalogNameList;
	   }
	   return QStringList();
}
QList<QPair<QString,QStringList>> readXML::readXml(QString filePath,QString selectedTagName)
{
	QList<QPair<QString,QStringList>> catalogList;
	QDomDocument doc;
	QString fileName=filePath;
//	QString fileName = QFileDialog::getOpenFileName(nullptr,"Open File", filePath, "(*.xml)");
	if("" == fileName)
    {
        qDebug()<<"Filename is Null";
        return QList<QPair<QString,QStringList>>();
    }
    QFile file(fileName);
	//QFile file(DirectorOf("xml").absoluteFilePath(fileName));
    if(!file.open(QFile::ReadOnly | QFile::Text))
        qDebug()<<"open file"<<fileName<<"failed, error:"<<file.errorString();
    /*����Dom�ڵ�*/
       QDomDocument    document;
       QString         strError;
       int             errLin = 0, errCol = 0;
       if( !document.setContent(&file, false, &strError, &errLin, &errCol) ) {
           qDebug()<<"parse file failed at line"<<errLin<<",column"<<errCol<<","<<strError;
           return QList<QPair<QString,QStringList>>();
       }
 
       if( document.isNull() ) {
           qDebug()<<"document is null !";
           return QList<QPair<QString,QStringList>>();
       }
 
       QDomElement root = document.documentElement();
       qDebug()<<root.tagName();
 
       QDomElement catalogs = root.firstChildElement();
       if( catalogs.isNull() )
           return QList<QPair<QString,QStringList>>();
       else
           qDebug()<<catalogs.tagName();
	   QDomNodeList nodelist=root.elementsByTagName(selectedTagName);
	   for(int k=0;k<nodelist.item(0).childNodes().count();k++)
       {
		   QDomNode node=nodelist.item(0);
		   QString tag = node.childNodes().at(k).childNodes().at(0).nodeName();
           QStringList child;
           QPair<QString,QStringList> pair;
		   for(int i = 0;i < node.childNodes().at(k).childNodes().size();i++)
			   child<<node.childNodes().at(k).childNodes().at(i).toElement().text();
           pair.first = tag;
           pair.second = child;
           catalogList.append(pair);
       }
       file.close();
	   return catalogList;
}
void readXML::writeXml(QString filePath,QTableWidget *table,QString nodeTagName,QString srcLayerName,QString targetLayerName)
{
	QString xmlPath = filePath;
	QFile file(xmlPath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	return ;
	QString errorStr;
	int errorLine;
	int errorColumn;
    /*��һ������file�е����ݶ���doc�ڴ���*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*�ڶ������������ڵ�*/
	QDomElement root = doc.documentElement();//.createElement("root");     //�������ڵ�
	/*�������������ӽڵ��Ԫ�أ�����Ԫ�ؼ�����Ӧ�ڵ��ڣ���󽫽ڵ������ڵ���*/
	if(doc.elementsByTagName(nodeTagName).length()>0)
	{
		QMessageBox::about(nullptr,"Info","�ñ�ǩ�����Ѵ��ڣ�������������");
		return;
	}
	QDomElement element = doc.createElement(nodeTagName);     //������һ���ӽڵ�
	element.setAttribute("caption","MapLayer Config");

	QDomElement el_vector = doc.createElement("VectorLayers");  //����һ��Ԫ�� ��Ԫ������������
	el_vector.setAttribute("caption","the names of the srcLayer and updateLayer");
	element.appendChild(el_vector);                      //����Ԫ�����ӵ���һ�ڵ���.

	QDomElement el_path = doc.createElement("name");       //�����ڶ���Ԫ��,
	QDomText text_path = doc.createTextNode(srcLayerName);         //������Ԫ�ص��ı�
	el_path.appendChild(text_path);                        //����Ԫ���ı����ӵ���Ԫ����

	el_vector.appendChild(el_path);                         //����Ԫ����ӵ���һ���ӽڵ���

	
	el_path = doc.createElement("name");      
	text_path = doc.createTextNode(targetLayerName);       
	el_path.appendChild(text_path);                        
	el_vector.appendChild(el_path);                         

	//�ڶ����ڵ�
	QDomElement el_selectedState = doc.createElement("selectedState");  //����һ��Ԫ�� ��Ԫ������������
	el_selectedState.setAttribute("caption","select the match fields");
	element.appendChild(el_selectedState);                      //����Ԫ�����ӵ���һ�ڵ���.
	for(int i=0;i<table->rowCount();i++)
	{
		if(table->item(i,0)->checkState()==Qt::Checked)
		{
			QDomElement el_id = doc.createElement("id");       //�����ڶ���Ԫ��,
			QDomText text_id = doc.createTextNode(QString::number(i));         //������Ԫ�ص��ı�
			el_id.appendChild(text_id);                        //����Ԫ���ı����ӵ���Ԫ����
			el_selectedState.appendChild(el_id);                         //����Ԫ����ӵ���2���ӽڵ���
		}
	}
	

	//��3���ڵ�
	QDomElement el_targetFields = doc.createElement("targetFields");  //����һ��Ԫ�� ��Ԫ������������
	el_targetFields.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_targetFields);                      //����Ԫ�����ӵ���һ�ڵ���.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //�����ڶ���Ԫ��,
		QDomText text_fieldName = doc.createTextNode(table->item(i,1)->text());         //������Ԫ�ص��ı�
		el_fieldName.appendChild(text_fieldName);                        //����Ԫ���ı����ӵ���Ԫ����
		el_targetFields.appendChild(el_fieldName);                         //����Ԫ����ӵ���2���ӽڵ���
	}
	
	//��4���ڵ�
	QDomElement el_srcFields = doc.createElement("srcFields");  //����һ��Ԫ�� ��Ԫ������������
	el_selectedState.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_srcFields);                      //����Ԫ�����ӵ���һ�ڵ���.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //�����ڶ���Ԫ��,
		QDomText text_fieldName = doc.createTextNode(table->item(i,2)->text());         //������Ԫ�ص��ı�
		el_fieldName.appendChild(text_fieldName);                        //����Ԫ���ı����ӵ���Ԫ����
		el_srcFields.appendChild(el_fieldName);                         //����Ԫ����ӵ���2���ӽڵ���

	}
	
	root.appendChild(element);                           //�����ӽڵ���뵽���ڵ���.


	QFile f(xmlPath);
	if(!f.open(QFile::WriteOnly | QFile::Text))
	return;
	QTextStream out(&f);
	QTextCodec * codec = QTextCodec::codecForLocale();
	out.setCodec(codec);
	doc.save(out,4,QDomNode::EncodingFromTextStream);
	f.close();
    QMessageBox::about(nullptr,"Info","��ӳɹ���");
}
void readXML::saveCurrentNode(QString filePath,QTableWidget *table,QString nodeTagName,QString srcLayerName,QString targetLayerName)
{
	QString xmlPath = filePath;
	QFile file(xmlPath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	return ;
	QString errorStr;
	int errorLine;
	int errorColumn;
    /*��һ������file�е����ݶ���doc�ڴ���*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*�ڶ������������ڵ�*/
	QDomElement root = doc.documentElement();//.createElement("root");     //�������ڵ�
	QDomNode node=root.firstChild();
	while(!node.isNull())
	{
		if(node.toElement().tagName()==nodeTagName)
			root.removeChild(node);
		node=node.nextSibling();
	}
	/*�������������ӽڵ��Ԫ�أ�����Ԫ�ؼ�����Ӧ�ڵ��ڣ���󽫽ڵ������ڵ���*/
	QDomElement element = doc.createElement(nodeTagName);     //������һ���ӽڵ�
	element.setAttribute("caption","MapLayer Config");

	QDomElement el_vector = doc.createElement("VectorLayers");  //����һ��Ԫ�� ��Ԫ������������
	el_vector.setAttribute("caption","the names of the srcLayer and updateLayer");
	element.appendChild(el_vector);                      //����Ԫ�����ӵ���һ�ڵ���.

	QDomElement el_path = doc.createElement("name");       //�����ڶ���Ԫ��,
	QDomText text_path = doc.createTextNode(srcLayerName);         //������Ԫ�ص��ı�
	el_path.appendChild(text_path);                        //����Ԫ���ı����ӵ���Ԫ����
	el_vector.appendChild(el_path);                         //����Ԫ����ӵ���һ���ӽڵ���

	el_path = doc.createElement("name");      
	text_path = doc.createTextNode(targetLayerName);       
	el_path.appendChild(text_path);                        
	el_vector.appendChild(el_path);                         

	//�ڶ����ڵ�
	QDomElement el_selectedState = doc.createElement("selectedState");  //����һ��Ԫ�� ��Ԫ������������
	el_selectedState.setAttribute("caption","select the match fields");
	element.appendChild(el_selectedState);                      //����Ԫ�����ӵ���һ�ڵ���.
	int count=table->rowCount();
	for(int i=0;i<table->rowCount();i++)
	{
		if(table->item(i,0)->checkState()==Qt::Checked)
		{
			QDomElement el_id = doc.createElement("id");       //�����ڶ���Ԫ��,
			QDomText text_id = doc.createTextNode(QString::number(i));         //������Ԫ�ص��ı�
			el_id.appendChild(text_id);                        //����Ԫ���ı����ӵ���Ԫ����
			el_selectedState.appendChild(el_id);                         //����Ԫ����ӵ���2���ӽڵ���
		}
	}
	//��3���ڵ�
	QDomElement el_targetFields = doc.createElement("targetFields");  //����һ��Ԫ�� ��Ԫ������������
	el_targetFields.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_targetFields);                      //����Ԫ�����ӵ���һ�ڵ���.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //�����ڶ���Ԫ��,
		QDomText text_fieldName = doc.createTextNode(table->item(i,1)->text());         //������Ԫ�ص��ı�
		el_fieldName.appendChild(text_fieldName);                        //����Ԫ���ı����ӵ���Ԫ����
		el_targetFields.appendChild(el_fieldName);                         //����Ԫ����ӵ���2���ӽڵ���
	}
	//��4���ڵ�
	QDomElement el_srcFields = doc.createElement("srcFields");  //����һ��Ԫ�� ��Ԫ������������
	el_selectedState.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_srcFields);                      //����Ԫ�����ӵ���һ�ڵ���.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //�����ڶ���Ԫ��,
		QDomText text_fieldName = doc.createTextNode(table->item(i,2)->text());         //������Ԫ�ص��ı�
		el_fieldName.appendChild(text_fieldName);                        //����Ԫ���ı����ӵ���Ԫ����
		el_srcFields.appendChild(el_fieldName);                         //����Ԫ����ӵ���2���ӽڵ���

	}
	
	root.appendChild(element);                           //�����ӽڵ���뵽���ڵ���.


	QFile f(xmlPath);
	if(!f.open(QFile::WriteOnly | QFile::Text))
	return;
	QTextStream out(&f);
	QTextCodec * codec = QTextCodec::codecForLocale();
	out.setCodec(codec);
	doc.save(out,4,QDomNode::EncodingFromTextStream);
	f.close();
    QMessageBox::about(nullptr,"Info","����ɹ���");
}
void readXML::deleteNode(QString filePath,QString nodeTagName)
{
	QString xmlPath = filePath;
	QFile file(xmlPath);
	QString errorStr;
	int errorLine;
	int errorColumn;
    /*��һ������file�е����ݶ���doc�ڴ���*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*�ڶ������������ڵ�*/
	QDomElement root = doc.documentElement();//.createElement("root");     //�������ڵ�
	QDomNodeList nodeList=doc.elementsByTagName(nodeTagName);
	for(int i=0;i<nodeList.count();i++)
	{
		QDomElement e=nodeList.at(i).toElement();
		root = doc.documentElement();  //ȡ�����ڵ�
        root.removeChild(nodeList.at(i));  //�Ӹ��ڵ���ɾ���ýڵ�
        qDebug() << "remove ok !";
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//ɾ��ʱҪ������д
            return ;
        QTextStream out(&file);
        doc.save(out,4);
        file.close();             
	}
	QMessageBox::about(nullptr,"Info","ɾ���ɹ���");
}
readXML::~readXML(void)
{
}
