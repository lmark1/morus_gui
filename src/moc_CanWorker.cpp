/****************************************************************************
** Meta object code from reading C++ file 'CanWorker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/CanWorker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CanWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CanWorker_t {
    QByteArrayData data[9];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CanWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CanWorker_t qt_meta_stringdata_CanWorker = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CanWorker"
QT_MOC_LITERAL(1, 10, 8), // "finished"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 5), // "error"
QT_MOC_LITERAL(4, 26, 3), // "err"
QT_MOC_LITERAL(5, 30, 20), // "nodeInformationFound"
QT_MOC_LITERAL(6, 51, 24), // "std::vector<NodeInfo_t>*"
QT_MOC_LITERAL(7, 76, 14), // "activeNodeInfo"
QT_MOC_LITERAL(8, 91, 7) // "process"

    },
    "CanWorker\0finished\0\0error\0err\0"
    "nodeInformationFound\0std::vector<NodeInfo_t>*\0"
    "activeNodeInfo\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CanWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       5,    1,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CanWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CanWorker *_t = static_cast<CanWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->nodeInformationFound((*reinterpret_cast< std::vector<NodeInfo_t>*(*)>(_a[1]))); break;
        case 3: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CanWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CanWorker::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (CanWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CanWorker::error)) {
                *result = 1;
            }
        }
        {
            typedef void (CanWorker::*_t)(std::vector<NodeInfo_t> * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CanWorker::nodeInformationFound)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CanWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CanWorker.data,
      qt_meta_data_CanWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CanWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CanWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CanWorker.stringdata0))
        return static_cast<void*>(const_cast< CanWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int CanWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CanWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CanWorker::error(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CanWorker::nodeInformationFound(std::vector<NodeInfo_t> * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
