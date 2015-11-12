/****************************************************************************
** Meta object code from reading C++ file 'data.h'
**
** Created: Sun Feb 26 19:48:33 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "data.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'data.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Data[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      29,    5,    5,    5, 0x05,
      64,    5,    5,    5, 0x05,
      99,    5,    5,    5, 0x05,
     123,    5,    5,    5, 0x05,
     152,    5,    5,    5, 0x05,
     179,    5,    5,    5, 0x05,
     205,    5,    5,    5, 0x05,
     227,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     302,  244,    5,    5, 0x0a,
     365,  351,    5,    5, 0x0a,
     391,    5,    5,    5, 0x0a,
     412,    5,    5,    5, 0x0a,
     431,    5,    5,    5, 0x0a,
     447,    5,    5,    5, 0x0a,
     462,    5,    5,    5, 0x0a,
     479,    5,    5,    5, 0x0a,
     505,    5,    5,    5, 0x0a,
     542,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Data[] = {
    "Data\0\0sendPersonalImfo(Data)\0"
    "signalPassWaveform(vector<double>)\0"
    "signalPassSpectrum(vector<double>)\0"
    "signalSaveDataSuccess()\0"
    "signalSaveParameterSuccess()\0"
    "signalParameterSaveError()\0"
    "signalRepeatImportError()\0"
    "signalImportSuccess()\0signalAutoStop()\0"
    "name_tmp,number_tmp,sex_tmp,height_tmp,weight_tmp,age_tmp\0"
    "keyToSure(string,string,sex_bool,int,double,int)\0"
    "isDyncDsp,feq\0beginMeasure(bool,double)\0"
    "getPersonalImfoACK()\0slotShowSpectrum()\0"
    "slotClearData()\0slotSaveData()\0"
    "slotEndMeasure()\0slotPassFileName(QString)\0"
    "slotThreadData(const vector<double>)\0"
    "slotImportResult()\0"
};

const QMetaObject Data::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Data,
      qt_meta_data_Data, 0 }
};

const QMetaObject *Data::metaObject() const
{
    return &staticMetaObject;
}

void *Data::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Data))
        return static_cast<void*>(const_cast< Data*>(this));
    return QWidget::qt_metacast(_clname);
}

int Data::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendPersonalImfo((*reinterpret_cast< const Data(*)>(_a[1]))); break;
        case 1: signalPassWaveform((*reinterpret_cast< const vector<double>(*)>(_a[1]))); break;
        case 2: signalPassSpectrum((*reinterpret_cast< const vector<double>(*)>(_a[1]))); break;
        case 3: signalSaveDataSuccess(); break;
        case 4: signalSaveParameterSuccess(); break;
        case 5: signalParameterSaveError(); break;
        case 6: signalRepeatImportError(); break;
        case 7: signalImportSuccess(); break;
        case 8: signalAutoStop(); break;
        case 9: keyToSure((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])),(*reinterpret_cast< sex_bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 10: beginMeasure((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 11: getPersonalImfoACK(); break;
        case 12: slotShowSpectrum(); break;
        case 13: slotClearData(); break;
        case 14: slotSaveData(); break;
        case 15: slotEndMeasure(); break;
        case 16: slotPassFileName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: slotThreadData((*reinterpret_cast< const vector<double>(*)>(_a[1]))); break;
        case 18: slotImportResult(); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void Data::sendPersonalImfo(const Data & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Data::signalPassWaveform(const vector<double> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Data::signalPassSpectrum(const vector<double> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Data::signalSaveDataSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Data::signalSaveParameterSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Data::signalParameterSaveError()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Data::signalRepeatImportError()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Data::signalImportSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Data::signalAutoStop()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
