/****************************************************************************
** Meta object code from reading C++ file 'IVFDCalibrate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IVFDCalibrate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IVFDCalibrate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IVFDCalibrate_t {
    QByteArrayData data[16];
    char stringdata[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IVFDCalibrate_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IVFDCalibrate_t qt_meta_stringdata_IVFDCalibrate = {
    {
QT_MOC_LITERAL(0, 0, 13), // "IVFDCalibrate"
QT_MOC_LITERAL(1, 14, 17), // "SIG_SetPushButton"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 2), // "id"
QT_MOC_LITERAL(4, 36, 4), // "flag"
QT_MOC_LITERAL(5, 41, 17), // "SIG_NotifyChannel"
QT_MOC_LITERAL(6, 59, 10), // "SLOT_Input"
QT_MOC_LITERAL(7, 70, 19), // "SLOT_StartCalibrate"
QT_MOC_LITERAL(8, 90, 19), // "SLOT_GetCurrentArgs"
QT_MOC_LITERAL(9, 110, 23), // "SLOT_RestoreDefaultArgs"
QT_MOC_LITERAL(10, 134, 20), // "SLOT_SaveCurrentArgs"
QT_MOC_LITERAL(11, 155, 16), // "SLOT_InputManual"
QT_MOC_LITERAL(12, 172, 20), // "SLOT_CalculateRealXY"
QT_MOC_LITERAL(13, 193, 14), // "SLOT_Calibrate"
QT_MOC_LITERAL(14, 208, 16), // "SLOT_ReadInputXY"
QT_MOC_LITERAL(15, 225, 18) // "SLOT_SetPushButton"

    },
    "IVFDCalibrate\0SIG_SetPushButton\0\0id\0"
    "flag\0SIG_NotifyChannel\0SLOT_Input\0"
    "SLOT_StartCalibrate\0SLOT_GetCurrentArgs\0"
    "SLOT_RestoreDefaultArgs\0SLOT_SaveCurrentArgs\0"
    "SLOT_InputManual\0SLOT_CalculateRealXY\0"
    "SLOT_Calibrate\0SLOT_ReadInputXY\0"
    "SLOT_SetPushButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IVFDCalibrate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   80,    2, 0x0a /* Public */,
       7,    0,   81,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x0a /* Public */,
       9,    0,   83,    2, 0x0a /* Public */,
      10,    0,   84,    2, 0x0a /* Public */,
      11,    0,   85,    2, 0x0a /* Public */,
      12,    0,   86,    2, 0x0a /* Public */,
      13,    0,   87,    2, 0x0a /* Public */,
      14,    0,   88,    2, 0x0a /* Public */,
      15,    2,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,

       0        // eod
};

void IVFDCalibrate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IVFDCalibrate *_t = static_cast<IVFDCalibrate *>(_o);
        switch (_id) {
        case 0: _t->SIG_SetPushButton((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->SIG_NotifyChannel(); break;
        case 2: _t->SLOT_Input(); break;
        case 3: _t->SLOT_StartCalibrate(); break;
        case 4: _t->SLOT_GetCurrentArgs(); break;
        case 5: _t->SLOT_RestoreDefaultArgs(); break;
        case 6: _t->SLOT_SaveCurrentArgs(); break;
        case 7: _t->SLOT_InputManual(); break;
        case 8: _t->SLOT_CalculateRealXY(); break;
        case 9: _t->SLOT_Calibrate(); break;
        case 10: _t->SLOT_ReadInputXY(); break;
        case 11: _t->SLOT_SetPushButton((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IVFDCalibrate::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDCalibrate::SIG_SetPushButton)) {
                *result = 0;
            }
        }
        {
            typedef void (IVFDCalibrate::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDCalibrate::SIG_NotifyChannel)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject IVFDCalibrate::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_IVFDCalibrate.data,
      qt_meta_data_IVFDCalibrate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IVFDCalibrate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IVFDCalibrate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IVFDCalibrate.stringdata))
        return static_cast<void*>(const_cast< IVFDCalibrate*>(this));
    return QDialog::qt_metacast(_clname);
}

int IVFDCalibrate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void IVFDCalibrate::SIG_SetPushButton(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IVFDCalibrate::SIG_NotifyChannel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
