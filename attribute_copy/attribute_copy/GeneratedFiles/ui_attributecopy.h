/********************************************************************************
** Form generated from reading UI file 'attributecopy.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTECOPY_H
#define UI_ATTRIBUTECOPY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AttributeCopyForm
{
public:
    QTabWidget *tabWidget;
    QWidget *multiLyrMode;
    QWidget *clipMode;
    QWidget *splitMode;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *sure_btn;
    QPushButton *close_btn;
    QLabel *valueNum;
    QGroupBox *groupBox;
    QComboBox *srcLyrList;
    QGroupBox *dataSetGroup;
    QHBoxLayout *horizontalLayout;
    QRadioButton *alldata_radio;
    QRadioButton *dataset_radio;
    QComboBox *updateLyrList;
    QLabel *srclyr_label;
    QSpinBox *numSpare;
    QLabel *updatelyr_label;
    QLabel *spareoverride_label;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QTableWidget *attribute_table;
    QPushButton *select_all;
    QPushButton *del_relation;
    QLabel *field_relation_label;
    QPushButton *auto_match;
    QPushButton *all_match;
    QPushButton *hand_match;
    QProgressBar *progressBar;
    QGroupBox *groupBox_3;
    QComboBox *cbxConfigList;
    QPushButton *addbtn;
    QLabel *label;
    QLineEdit *inputTagName;
    QPushButton *savebtn;
    QPushButton *delbtn;

    void setupUi(QWidget *AttributeCopyForm)
    {
        if (AttributeCopyForm->objectName().isEmpty())
            AttributeCopyForm->setObjectName(QString::fromUtf8("AttributeCopyForm"));
        AttributeCopyForm->setEnabled(true);
        AttributeCopyForm->resize(790, 575);
        AttributeCopyForm->setMaximumSize(QSize(790, 607));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font.setPointSize(12);
        AttributeCopyForm->setFont(font);
        tabWidget = new QTabWidget(AttributeCopyForm);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 781, 31));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(21);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setSizeIncrement(QSize(0, 0));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        tabWidget->setFont(font1);
        tabWidget->setMouseTracking(false);
        tabWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(32, 32));
        tabWidget->setElideMode(Qt::ElideLeft);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        multiLyrMode = new QWidget();
        multiLyrMode->setObjectName(QString::fromUtf8("multiLyrMode"));
        multiLyrMode->setBaseSize(QSize(12, 0));
        multiLyrMode->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase);
        tabWidget->addTab(multiLyrMode, QString());
        clipMode = new QWidget();
        clipMode->setObjectName(QString::fromUtf8("clipMode"));
        tabWidget->addTab(clipMode, QString());
        splitMode = new QWidget();
        splitMode->setObjectName(QString::fromUtf8("splitMode"));
        tabWidget->addTab(splitMode, QString());
        horizontalLayoutWidget_5 = new QWidget(AttributeCopyForm);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(540, 522, 210, 41));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        sure_btn = new QPushButton(horizontalLayoutWidget_5);
        sure_btn->setObjectName(QString::fromUtf8("sure_btn"));
        sure_btn->setMaximumSize(QSize(16777215, 31));
        sure_btn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(sure_btn);

        close_btn = new QPushButton(horizontalLayoutWidget_5);
        close_btn->setObjectName(QString::fromUtf8("close_btn"));
        close_btn->setMaximumSize(QSize(16777215, 31));
        close_btn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(close_btn);

        valueNum = new QLabel(AttributeCopyForm);
        valueNum->setObjectName(QString::fromUtf8("valueNum"));
        valueNum->setGeometry(QRect(459, 535, 80, 20));
        groupBox = new QGroupBox(AttributeCopyForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(4, 37, 780, 81));
        srcLyrList = new QComboBox(groupBox);
        srcLyrList->setObjectName(QString::fromUtf8("srcLyrList"));
        srcLyrList->setGeometry(QRect(110, 45, 650, 22));
        srcLyrList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        srcLyrList->setEditable(true);
        dataSetGroup = new QGroupBox(groupBox);
        dataSetGroup->setObjectName(QString::fromUtf8("dataSetGroup"));
        dataSetGroup->setGeometry(QRect(280, 84, 251, 41));
        dataSetGroup->setAutoFillBackground(false);
        dataSetGroup->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(dataSetGroup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        alldata_radio = new QRadioButton(dataSetGroup);
        alldata_radio->setObjectName(QString::fromUtf8("alldata_radio"));
        alldata_radio->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(alldata_radio);

        dataset_radio = new QRadioButton(dataSetGroup);
        dataset_radio->setObjectName(QString::fromUtf8("dataset_radio"));
        dataset_radio->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(dataset_radio);

        updateLyrList = new QComboBox(groupBox);
        updateLyrList->setObjectName(QString::fromUtf8("updateLyrList"));
        updateLyrList->setGeometry(QRect(110, 10, 650, 22));
        updateLyrList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        updateLyrList->setEditable(true);
        srclyr_label = new QLabel(groupBox);
        srclyr_label->setObjectName(QString::fromUtf8("srclyr_label"));
        srclyr_label->setGeometry(QRect(9, 44, 81, 20));
        srclyr_label->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));
        numSpare = new QSpinBox(groupBox);
        numSpare->setObjectName(QString::fromUtf8("numSpare"));
        numSpare->setGeometry(QRect(137, 98, 41, 22));
        numSpare->setMaximum(100);
        numSpare->setValue(0);
        updatelyr_label = new QLabel(groupBox);
        updatelyr_label->setObjectName(QString::fromUtf8("updatelyr_label"));
        updatelyr_label->setGeometry(QRect(10, 12, 91, 20));
        updatelyr_label->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));
        spareoverride_label = new QLabel(groupBox);
        spareoverride_label->setObjectName(QString::fromUtf8("spareoverride_label"));
        spareoverride_label->setGeometry(QRect(16, 98, 111, 21));
        spareoverride_label->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(180, 100, 16, 20));
        label_4->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));
        groupBox_2 = new QGroupBox(AttributeCopyForm);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(4, 171, 780, 351));
        attribute_table = new QTableWidget(groupBox_2);
        if (attribute_table->columnCount() < 3)
            attribute_table->setColumnCount(3);
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font2);
        attribute_table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font2);
        attribute_table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font2);
        attribute_table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        attribute_table->setObjectName(QString::fromUtf8("attribute_table"));
        attribute_table->setGeometry(QRect(4, 30, 770, 271));
        attribute_table->setMouseTracking(false);
        attribute_table->setAcceptDrops(false);
        attribute_table->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\345\256\213\344\275\223\";"));
        attribute_table->setDragEnabled(false);
        attribute_table->setAlternatingRowColors(false);
        attribute_table->setSelectionBehavior(QAbstractItemView::SelectItems);
        attribute_table->setRowCount(0);
        attribute_table->setColumnCount(3);
        attribute_table->horizontalHeader()->setDefaultSectionSize(240);
        attribute_table->horizontalHeader()->setMinimumSectionSize(21);
        select_all = new QPushButton(groupBox_2);
        select_all->setObjectName(QString::fromUtf8("select_all"));
        select_all->setGeometry(QRect(10, 307, 102, 31));
        select_all->setMaximumSize(QSize(16777215, 31));
        select_all->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        del_relation = new QPushButton(groupBox_2);
        del_relation->setObjectName(QString::fromUtf8("del_relation"));
        del_relation->setGeometry(QRect(120, 307, 101, 31));
        del_relation->setMaximumSize(QSize(16777215, 31));
        del_relation->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        field_relation_label = new QLabel(groupBox_2);
        field_relation_label->setObjectName(QString::fromUtf8("field_relation_label"));
        field_relation_label->setGeometry(QRect(10, 5, 81, 20));
        field_relation_label->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\256\213\344\275\223\";"));
        auto_match = new QPushButton(groupBox_2);
        auto_match->setObjectName(QString::fromUtf8("auto_match"));
        auto_match->setGeometry(QRect(530, 311, 99, 31));
        auto_match->setMaximumSize(QSize(16777215, 31));
        auto_match->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        all_match = new QPushButton(groupBox_2);
        all_match->setObjectName(QString::fromUtf8("all_match"));
        all_match->setGeometry(QRect(640, 310, 99, 31));
        all_match->setMaximumSize(QSize(16777215, 31));
        all_match->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        hand_match = new QPushButton(groupBox_2);
        hand_match->setObjectName(QString::fromUtf8("hand_match"));
        hand_match->setGeometry(QRect(423, 310, 99, 31));
        hand_match->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        progressBar = new QProgressBar(AttributeCopyForm);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(166, 532, 290, 23));
        progressBar->setAutoFillBackground(false);
        progressBar->setMinimum(0);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        groupBox_3 = new QGroupBox(AttributeCopyForm);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(3, 125, 781, 41));
        cbxConfigList = new QComboBox(groupBox_3);
        cbxConfigList->setObjectName(QString::fromUtf8("cbxConfigList"));
        cbxConfigList->setGeometry(QRect(130, 9, 101, 22));
        cbxConfigList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 10pt \"\345\256\213\344\275\223\";"));
        addbtn = new QPushButton(groupBox_3);
        addbtn->setObjectName(QString::fromUtf8("addbtn"));
        addbtn->setGeometry(QRect(630, 5, 91, 32));
        addbtn->setMaximumSize(QSize(16777215, 32));
        addbtn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 91, 21));
        inputTagName = new QLineEdit(groupBox_3);
        inputTagName->setObjectName(QString::fromUtf8("inputTagName"));
        inputTagName->setGeometry(QRect(480, 10, 141, 23));
        savebtn = new QPushButton(groupBox_3);
        savebtn->setObjectName(QString::fromUtf8("savebtn"));
        savebtn->setGeometry(QRect(240, 5, 81, 32));
        savebtn->setMaximumSize(QSize(105, 32));
        savebtn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        savebtn->setAutoDefault(false);
        delbtn = new QPushButton(groupBox_3);
        delbtn->setObjectName(QString::fromUtf8("delbtn"));
        delbtn->setGeometry(QRect(330, 5, 81, 32));
        delbtn->setMaximumSize(QSize(105, 32));
        delbtn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\256\213\344\275\223\";"));
        delbtn->setAutoDefault(false);

        retranslateUi(AttributeCopyForm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AttributeCopyForm);
    } // setupUi

    void retranslateUi(QWidget *AttributeCopyForm)
    {
        AttributeCopyForm->setWindowTitle(QApplication::translate("AttributeCopyForm", "\345\261\236\346\200\247\346\213\267\350\264\235", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(multiLyrMode), QApplication::translate("AttributeCopyForm", "\345\233\276\345\261\202\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(clipMode), QApplication::translate("AttributeCopyForm", "\345\233\276\345\261\202\351\227\264\350\243\201\345\211\252", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(splitMode), QApplication::translate("AttributeCopyForm", "\345\233\276\345\261\202\351\227\264\345\210\206\345\211\262", 0, QApplication::UnicodeUTF8));
        sure_btn->setText(QApplication::translate("AttributeCopyForm", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        close_btn->setText(QApplication::translate("AttributeCopyForm", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
        valueNum->setText(QApplication::translate("AttributeCopyForm", "0", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        alldata_radio->setText(QApplication::translate("AttributeCopyForm", "\346\211\200\346\234\211\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        dataset_radio->setText(QApplication::translate("AttributeCopyForm", "\351\200\211\346\213\251\351\233\206\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        srclyr_label->setText(QApplication::translate("AttributeCopyForm", "\346\272\220 \345\233\276 \345\261\202\357\274\232", 0, QApplication::UnicodeUTF8));
        updatelyr_label->setText(QApplication::translate("AttributeCopyForm", "\347\233\256\346\240\207\345\233\276\345\261\202\357\274\232", 0, QApplication::UnicodeUTF8));
        spareoverride_label->setText(QApplication::translate("AttributeCopyForm", "\351\235\242\347\247\257\351\207\215\345\217\240\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AttributeCopyForm", "%", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = attribute_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AttributeCopyForm", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = attribute_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AttributeCopyForm", "\346\233\264\346\226\260\345\233\276\345\261\202\345\255\227\346\256\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = attribute_table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AttributeCopyForm", "\346\225\260\346\215\256\346\272\220\345\233\276\345\261\202\345\255\227\346\256\265", 0, QApplication::UnicodeUTF8));
        select_all->setText(QApplication::translate("AttributeCopyForm", "\345\205\250\351\200\211", 0, QApplication::UnicodeUTF8));
        del_relation->setText(QApplication::translate("AttributeCopyForm", "\345\210\240\351\231\244\345\205\263\347\263\273", 0, QApplication::UnicodeUTF8));
        field_relation_label->setText(QApplication::translate("AttributeCopyForm", "\345\255\227\346\256\265\345\205\263\347\263\273", 0, QApplication::UnicodeUTF8));
        auto_match->setText(QApplication::translate("AttributeCopyForm", "\350\207\252\345\212\250\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        all_match->setText(QApplication::translate("AttributeCopyForm", "\345\205\250\351\203\250\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        hand_match->setText(QApplication::translate("AttributeCopyForm", "\346\211\213\345\267\245\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        addbtn->setText(QApplication::translate("AttributeCopyForm", "\346\267\273\345\212\240\346\226\260\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AttributeCopyForm", "\350\257\267\351\200\211\346\213\251\351\205\215\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        inputTagName->setToolTip(QApplication::translate("AttributeCopyForm", "<html><head/><body><p>\350\257\267\350\276\223\345\205\245\346\226\260\351\205\215\347\275\256\345\220\215\347\247\260</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        inputTagName->setWhatsThis(QApplication::translate("AttributeCopyForm", "<html><head/><body><p>\350\257\267\350\276\223\345\205\245\346\226\260\351\205\215\347\275\256\345\220\215\347\247\260\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        savebtn->setToolTip(QApplication::translate("AttributeCopyForm", "\344\277\235\345\255\230\344\277\256\346\224\271\345\220\216\347\232\204\345\267\262\346\234\211\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        savebtn->setWhatsThis(QApplication::translate("AttributeCopyForm", "\344\277\235\345\255\230\344\277\256\346\224\271\345\220\216\347\232\204\345\267\262\346\234\211\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        savebtn->setText(QApplication::translate("AttributeCopyForm", "\344\277\235\345\255\230\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        delbtn->setToolTip(QApplication::translate("AttributeCopyForm", "\345\210\240\351\231\244\345\267\262\346\234\211\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        delbtn->setWhatsThis(QApplication::translate("AttributeCopyForm", "\345\210\240\351\231\244\345\267\262\346\234\211\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        delbtn->setText(QApplication::translate("AttributeCopyForm", "\345\210\240\351\231\244\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AttributeCopyForm: public Ui_AttributeCopyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTECOPY_H
