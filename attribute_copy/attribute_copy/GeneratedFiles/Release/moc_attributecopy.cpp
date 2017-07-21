/****************************************************************************
** Meta object code from reading C++ file 'attributecopy.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../attributecopy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'attributecopy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_attributecopy[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      39,   14,   14,   14, 0x08,
      63,   14,   14,   14, 0x08,
      92,   14,   14,   14, 0x08,
     114,   14,   14,   14, 0x08,
     136,   14,   14,   14, 0x08,
     157,   14,   14,   14, 0x08,
     189,  184,   14,   14, 0x08,
     221,  184,   14,   14, 0x08,
     256,  184,   14,   14, 0x08,
     294,  184,   14,   14, 0x08,
     346,  335,   14,   14, 0x08,
     376,   14,   14,   14, 0x08,
     398,   14,   14,   14, 0x08,
     414,   14,   14,   14, 0x08,
     425,  335,   14,   14, 0x08,
     454,  184,   14,   14, 0x08,
     480,  184,   14,   14, 0x08,
     506,   14,   14,   14, 0x08,
     523,   14,   14,   14, 0x08,
     534,   14,   14,   14, 0x08,
     546,   14,   14,   14, 0x08,
     566,  560,   14,   14, 0x08,
     599,  184,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_attributecopy[] = {
    "attributecopy\0\0on_buttonBox_accepted()\0"
    "on_buttonBox_rejected()\0"
    "on_buttonBox_helpRequested()\0"
    "on_hand_match_click()\0on_auto_match_click()\0"
    "on_all_match_click()\0on_delete_relation_click()\0"
    "text\0on_srcComBox_activated(QString)\0"
    "on_updateComBox_activated(QString)\0"
    "on_srcComBox_EditTextChanged(QString)\0"
    "on_updateComBox_EditTextChanged(QString)\0"
    "row,column\0on_table_cellclicked(int,int)\0"
    "on_select_all_click()\0on_sure_click()\0"
    "on_close()\0on_checkbox_changed(int,int)\0"
    "on_src_activated(QString)\0"
    "on_upd_activated(QString)\0onRadioClicked()\0"
    "onAddSet()\0onSaveSet()\0onDeleteSet()\0"
    "index\0on_TabWidget_CurrentChanged(int)\0"
    "on_cbxConfigList_activated(QString)\0"
};

void attributecopy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        attributecopy *_t = static_cast<attributecopy *>(_o);
        switch (_id) {
        case 0: _t->on_buttonBox_accepted(); break;
        case 1: _t->on_buttonBox_rejected(); break;
        case 2: _t->on_buttonBox_helpRequested(); break;
        case 3: _t->on_hand_match_click(); break;
        case 4: _t->on_auto_match_click(); break;
        case 5: _t->on_all_match_click(); break;
        case 6: _t->on_delete_relation_click(); break;
        case 7: _t->on_srcComBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_updateComBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->on_srcComBox_EditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->on_updateComBox_EditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->on_table_cellclicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->on_select_all_click(); break;
        case 13: _t->on_sure_click(); break;
        case 14: _t->on_close(); break;
        case 15: _t->on_checkbox_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->on_src_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->on_upd_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->onRadioClicked(); break;
        case 19: _t->onAddSet(); break;
        case 20: _t->onSaveSet(); break;
        case 21: _t->onDeleteSet(); break;
        case 22: _t->on_TabWidget_CurrentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->on_cbxConfigList_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData attributecopy::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject attributecopy::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_attributecopy,
      qt_meta_data_attributecopy, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &attributecopy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *attributecopy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *attributecopy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_attributecopy))
        return static_cast<void*>(const_cast< attributecopy*>(this));
    return QDialog::qt_metacast(_clname);
}

int attributecopy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
