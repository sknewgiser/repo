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
    /*解析Dom节点*/
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
    /*解析Dom节点*/
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
    /*第一步，将file中的内容读入doc内存中*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*第二步，创建根节点*/
	QDomElement root = doc.documentElement();//.createElement("root");     //创建根节点
	/*第三步：创建子节点和元素，并将元素加入相应节点内，最后将节点加入根节点中*/
	if(doc.elementsByTagName(nodeTagName).length()>0)
	{
		QMessageBox::about(nullptr,"Info","该标签名称已存在，请重新命名！");
		return;
	}
	QDomElement element = doc.createElement(nodeTagName);     //创建第一个子节点
	element.setAttribute("caption","MapLayer Config");

	QDomElement el_vector = doc.createElement("VectorLayers");  //创建一个元素 该元素有三种属性
	el_vector.setAttribute("caption","the names of the srcLayer and updateLayer");
	element.appendChild(el_vector);                      //将该元素增加到第一节点中.

	QDomElement el_path = doc.createElement("name");       //创建第二个元素,
	QDomText text_path = doc.createTextNode(srcLayerName);         //创建该元素的文本
	el_path.appendChild(text_path);                        //将该元素文本增加到该元素中

	el_vector.appendChild(el_path);                         //将该元素添加到第一个子节点中

	
	el_path = doc.createElement("name");      
	text_path = doc.createTextNode(targetLayerName);       
	el_path.appendChild(text_path);                        
	el_vector.appendChild(el_path);                         

	//第二个节点
	QDomElement el_selectedState = doc.createElement("selectedState");  //创建一个元素 该元素有三种属性
	el_selectedState.setAttribute("caption","select the match fields");
	element.appendChild(el_selectedState);                      //将该元素增加到第一节点中.
	for(int i=0;i<table->rowCount();i++)
	{
		if(table->item(i,0)->checkState()==Qt::Checked)
		{
			QDomElement el_id = doc.createElement("id");       //创建第二个元素,
			QDomText text_id = doc.createTextNode(QString::number(i));         //创建该元素的文本
			el_id.appendChild(text_id);                        //将该元素文本增加到该元素中
			el_selectedState.appendChild(el_id);                         //将该元素添加到第2个子节点中
		}
	}
	

	//第3个节点
	QDomElement el_targetFields = doc.createElement("targetFields");  //创建一个元素 该元素有三种属性
	el_targetFields.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_targetFields);                      //将该元素增加到第一节点中.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //创建第二个元素,
		QDomText text_fieldName = doc.createTextNode(table->item(i,1)->text());         //创建该元素的文本
		el_fieldName.appendChild(text_fieldName);                        //将该元素文本增加到该元素中
		el_targetFields.appendChild(el_fieldName);                         //将该元素添加到第2个子节点中
	}
	
	//第4个节点
	QDomElement el_srcFields = doc.createElement("srcFields");  //创建一个元素 该元素有三种属性
	el_selectedState.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_srcFields);                      //将该元素增加到第一节点中.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //创建第二个元素,
		QDomText text_fieldName = doc.createTextNode(table->item(i,2)->text());         //创建该元素的文本
		el_fieldName.appendChild(text_fieldName);                        //将该元素文本增加到该元素中
		el_srcFields.appendChild(el_fieldName);                         //将该元素添加到第2个子节点中

	}
	
	root.appendChild(element);                           //将该子节点加入到根节点下.


	QFile f(xmlPath);
	if(!f.open(QFile::WriteOnly | QFile::Text))
	return;
	QTextStream out(&f);
	QTextCodec * codec = QTextCodec::codecForLocale();
	out.setCodec(codec);
	doc.save(out,4,QDomNode::EncodingFromTextStream);
	f.close();
    QMessageBox::about(nullptr,"Info","添加成功！");
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
    /*第一步，将file中的内容读入doc内存中*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*第二步，创建根节点*/
	QDomElement root = doc.documentElement();//.createElement("root");     //创建根节点
	QDomNode node=root.firstChild();
	while(!node.isNull())
	{
		if(node.toElement().tagName()==nodeTagName)
			root.removeChild(node);
		node=node.nextSibling();
	}
	/*第三步：创建子节点和元素，并将元素加入相应节点内，最后将节点加入根节点中*/
	QDomElement element = doc.createElement(nodeTagName);     //创建第一个子节点
	element.setAttribute("caption","MapLayer Config");

	QDomElement el_vector = doc.createElement("VectorLayers");  //创建一个元素 该元素有三种属性
	el_vector.setAttribute("caption","the names of the srcLayer and updateLayer");
	element.appendChild(el_vector);                      //将该元素增加到第一节点中.

	QDomElement el_path = doc.createElement("name");       //创建第二个元素,
	QDomText text_path = doc.createTextNode(srcLayerName);         //创建该元素的文本
	el_path.appendChild(text_path);                        //将该元素文本增加到该元素中
	el_vector.appendChild(el_path);                         //将该元素添加到第一个子节点中

	el_path = doc.createElement("name");      
	text_path = doc.createTextNode(targetLayerName);       
	el_path.appendChild(text_path);                        
	el_vector.appendChild(el_path);                         

	//第二个节点
	QDomElement el_selectedState = doc.createElement("selectedState");  //创建一个元素 该元素有三种属性
	el_selectedState.setAttribute("caption","select the match fields");
	element.appendChild(el_selectedState);                      //将该元素增加到第一节点中.
	int count=table->rowCount();
	for(int i=0;i<table->rowCount();i++)
	{
		if(table->item(i,0)->checkState()==Qt::Checked)
		{
			QDomElement el_id = doc.createElement("id");       //创建第二个元素,
			QDomText text_id = doc.createTextNode(QString::number(i));         //创建该元素的文本
			el_id.appendChild(text_id);                        //将该元素文本增加到该元素中
			el_selectedState.appendChild(el_id);                         //将该元素添加到第2个子节点中
		}
	}
	//第3个节点
	QDomElement el_targetFields = doc.createElement("targetFields");  //创建一个元素 该元素有三种属性
	el_targetFields.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_targetFields);                      //将该元素增加到第一节点中.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //创建第二个元素,
		QDomText text_fieldName = doc.createTextNode(table->item(i,1)->text());         //创建该元素的文本
		el_fieldName.appendChild(text_fieldName);                        //将该元素文本增加到该元素中
		el_targetFields.appendChild(el_fieldName);                         //将该元素添加到第2个子节点中
	}
	//第4个节点
	QDomElement el_srcFields = doc.createElement("srcFields");  //创建一个元素 该元素有三种属性
	el_selectedState.setAttribute("caption","same fields between targetLayer and srcLayer");
	element.appendChild(el_srcFields);                      //将该元素增加到第一节点中.
	for(int i=0;i<table->rowCount();i++)
	{
		QDomElement el_fieldName = doc.createElement("fieldName");       //创建第二个元素,
		QDomText text_fieldName = doc.createTextNode(table->item(i,2)->text());         //创建该元素的文本
		el_fieldName.appendChild(text_fieldName);                        //将该元素文本增加到该元素中
		el_srcFields.appendChild(el_fieldName);                         //将该元素添加到第2个子节点中

	}
	
	root.appendChild(element);                           //将该子节点加入到根节点下.


	QFile f(xmlPath);
	if(!f.open(QFile::WriteOnly | QFile::Text))
	return;
	QTextStream out(&f);
	QTextCodec * codec = QTextCodec::codecForLocale();
	out.setCodec(codec);
	doc.save(out,4,QDomNode::EncodingFromTextStream);
	f.close();
    QMessageBox::about(nullptr,"Info","保存成功！");
}
void readXML::deleteNode(QString filePath,QString nodeTagName)
{
	QString xmlPath = filePath;
	QFile file(xmlPath);
	QString errorStr;
	int errorLine;
	int errorColumn;
    /*第一步，将file中的内容读入doc内存中*/  
	QDomDocument doc;
	doc.setContent(&file,false,&errorStr,&errorLine,&errorColumn);
	file.close();
	/*第二步，创建根节点*/
	QDomElement root = doc.documentElement();//.createElement("root");     //创建根节点
	QDomNodeList nodeList=doc.elementsByTagName(nodeTagName);
	for(int i=0;i<nodeList.count();i++)
	{
		QDomElement e=nodeList.at(i).toElement();
		root = doc.documentElement();  //取出根节点
        root.removeChild(nodeList.at(i));  //从根节点上删除该节点
        qDebug() << "remove ok !";
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//删除时要覆盖重写
            return ;
        QTextStream out(&file);
        doc.save(out,4);
        file.close();             
	}
	QMessageBox::about(nullptr,"Info","删除成功！");
}
readXML::~readXML(void)
{
}
