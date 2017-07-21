/****************************************************************************
** Meta object code from reading C++ file 'attribute_copy.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../attribute_copy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'attribute_copy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_attribute_copy[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      26,   15,   15,   15, 0x0a,
      32,   15,   15,   15, 0x0a,
      41,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_attribute_copy[] = {
    "attribute_copy\0\0initGui()\0run()\0"
    "unload()\0help()\0"
};

void attribute_copy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        attribute_copy *_t = static_cast<attribute_copy *>(_o);
        switch (_id) {
        case 0: _t->initGui(); break;
        case 1: _t->run(); break;
        case 2: _t->unload(); break;
        case 3: _t->help(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData attribute_copy::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject attribute_copy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_attribute_copy,
      qt_meta_data_attribute_copy, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &attribute_copy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *attribute_copy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *attribute_copy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_attribute_copy))
        return static_cast<void*>(const_cast< attribute_copy*>(this));
    if (!strcmp(_clname, "QgisPlugin"))
        return static_cast< QgisPlugin*>(const_cast< attribute_copy*>(this));
    return QObject::qt_metacast(_clname);
}

int attribute_copy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
