/****************************************************************************
** Meta object code from reading C++ file 'MorusMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/MorusMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MorusMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MorusMainWindow_t {
    QByteArrayData data[9];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MorusMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MorusMainWindow_t qt_meta_stringdata_MorusMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MorusMainWindow"
QT_MOC_LITERAL(1, 16, 31), // "on_startLocalNodeButton_clicked"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 18), // "handleErrorMessage"
QT_MOC_LITERAL(4, 68, 5), // "error"
QT_MOC_LITERAL(5, 74, 14), // "workerFinished"
QT_MOC_LITERAL(6, 89, 16), // "updateCanMonitor"
QT_MOC_LITERAL(7, 106, 11), // "NodeInfo_t*"
QT_MOC_LITERAL(8, 118, 8) // "nodeInfo"

    },
    "MorusMainWindow\0on_startLocalNodeButton_clicked\0"
    "\0handleErrorMessage\0error\0workerFinished\0"
    "updateCanMonitor\0NodeInfo_t*\0nodeInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MorusMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       6,    1,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void MorusMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MorusMainWindow *_t = static_cast<MorusMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_startLocalNodeButton_clicked(); break;
        case 1: _t->handleErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->workerFinished(); break;
        case 3: _t->updateCanMonitor((*reinterpret_cast< NodeInfo_t*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MorusMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MorusMainWindow.data,
      qt_meta_data_MorusMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MorusMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MorusMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MorusMainWindow.stringdata0))
        return static_cast<void*>(const_cast< MorusMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MorusMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
