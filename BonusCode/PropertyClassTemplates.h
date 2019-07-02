#ifndef PROPERTYCLASSTEMPLATES_H
#define PROPERTYCLASSTEMPLATES_H

/**
 * This code is released AS-IS. Bugs and other issues may exist.  The fitness
 * of this code for use is not assured.
 * 
 * In other words: You are responsible for ensuring the code works properly in
 * your project.
 * 
 **/

#include <type_traits>
#include <QObject>
#include <QList>
#include <QByteArray>
#include <QMetaObject>
#include <QMetaProperty>
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include <QJsonObject>

/**
 * @def PROPERTY(Type, Name)
 *
 * @brief   A macro to define a class "Property" (Q_PROPERTY).
 * @param   Type    - the type of the property
 * @param   Name    - the name of the property
 *
 * This macro defines a Q_PROPERTY, its storage, and the required getter and
 * setter methods.  This allows the property to be accessed by through the
 * QObject or Q_GADGET system.  It also makes importing and exporting the class
 * to XML Streams very easy as the property system does most of the work for you.
 *
 * @note    This can only be used with a class that inherits from QObject or Q_GADGET
 */
#define PROPERTY(Type, Name)   \
    private:                                                                  \
        Type m_##Name;                                                        \
        Q_PROPERTY(Type Name READ Name WRITE set##Name)  \
    public:                                                                   \
        void set##Name(const Type &value) { m_##Name = value; }               \
        /*void set##Name(const QVariant &value) { set##Name(value.value<Type>()); } */ \
        Type Name() const { return m_##Name; }

/**
 * @def NOTIFYING_PROPERTY(Type, Name)
 *
 * @brief   A macro to define a class "Property" (Q_PROPERTY).
 * @param   Type    - the type of the property
 * @param   Name    - the name of the property
 *
 * This macro defines a Q_PROPERTY, its storage, and the required getter and
 * setter methods.  This allows the property to be accessed by through the
 * QObject or Q_GADGET system.  It also makes importing and exporting the class
 * to XML Streams very easy as the property system does most of the work for you.
 *
 * @note    This can only be used with a class that inherits from QObject or Q_GADGET
 */
#define NOTIFYING_PROPERTY(Type, Name)   \
    private:                                                                  \
        Type m_##Name;                                                        \
        Q_PROPERTY(Type Name READ Name WRITE set##Name NOTIFY Name ## Changed)  \
    public:                                                                   \
        void set##Name(const Type value) { m_##Name = value; emit Name ## Changed();}               \
        void set##Name(const QVariant value) { set##Name(value.value<Type>()); } \
        Type Name() const { return m_##Name; }

/**
 * @def NOTIFYING_POINTER_PROPERTY(Type, Name)
 *
 * @brief   A macro to define a class "Property" (Q_PROPERTY).
 * @param   Type    - the type of the property
 * @param   Name    - the name of the property
 *
 * This macro defines a Q_PROPERTY, its storage, and the required getter and
 * setter methods.  This allows the property to be accessed by through the
 * QObject or Q_GADGET system.  It also makes importing and exporting the class
 * to XML Streams very easy as the property system does most of the work for you.
 *
 * @note    This can only be used with a class that inherits from QObject or Q_GADGET
 */
#define NOTIFYING_POINTER_PROPERTY(Type, Name)   \
    private:                                                                  \
        Type *m_##Name;                                                       \
        Q_PROPERTY(Type * Name READ Name WRITE set##Name NOTIFY Name ## Changed)  \
    public:                                                                   \
        void set##Name(const Type *value) { *m_##Name = *value; emit Name ## Changed();}               \
        void set##Name(const QVariant value) { *m_##Name = value.value<Type>(); } \
        Type *Name() const { return m_##Name; }

/**
 * @def REGISTER_PROPERTY_FIELD(FieldName, PropName, Widget)
 *
 * @brief   A macro for registering a field (like used in QWizardPages)
 * @param   FieldName   - the name the field will be known by
 * @param   PropName    - the name of the property as specified in the PROPERTY(..)
 * @param   Widget      - a pointer to the instantiated widget with the PROPERTY
 *
 * This convenience macro produces a call to the registerField(..) method that
 * uses the same naming schemes as the PROPERTY(..) macro.
 *
 * @note    "registerField(..)" must be defined in the context where this macro
 *          is used (i.e. in a QWizardPage).
 */
#define REGISTER_PROPERTY_FIELD(FieldName, PropName, Widget)    \
        registerField("FieldName", Widget, "PropName", "PropName##Changed");

/**
 * @def XMLWRITE_SIMPLE_PROPERTY(XMLWriter, Property, Source)
 *
 * @brief   A simple macro to make writing out XML properties a bit easier
 *
 * @param   XMLWriter - the QXmlStreamWriter to write to
 * @param   Property  - the name of the property to write
 * @param   Source    - the item containing the property
 */
#define XMLWRITE_SIMPLE_PROPERTY(XMLWriter, Property, Source) \
    XMLWriter.writeTextElement(Property, __GetProperty<decltype (Source)>(Source, Property).toString())


/// For getting properties from a Q_GADGET
template <typename OT>
QVariant __GetPropValue(const QMetaProperty &prop, const OT &obj, std::false_type)
{
    return prop.readOnGadget(&obj);
}

/// For getting property values from a QObject
template <typename OT>
QVariant __GetPropValue(const QMetaProperty &prop, const OT &obj, std::true_type)
{
    return prop.read(&obj);
}

/**
 * @brief Generic way to get property from Object or Q_GADGET.
 *
 * This uses a type check to choose one of two methods based on if obj
 * inherits from QObject or not.
 *
 */
template <typename OT>
QVariant __GetPropValue(const QMetaProperty &prop, const OT &obj)
{
    return __GetPropValue(prop,
                          obj,
                          typename std::is_base_of<QObject, OT>::type());
}

/// For setting properties from a Q_GADGET
template <typename OT>
QVariant __SetPropValue(const QMetaProperty &prop, const OT &obj, std::false_type)
{
    return prop.readOnGadget(&obj);
}

/// For getting property values from a QObject
template <typename OT>
QVariant __SetPropValue(const QMetaProperty &prop, const OT &obj, std::true_type)
{
    return prop.read(&obj);
}

/**
 * @brief Generic way to get property from Object or Q_GADGET.
 *
 * This uses a type check to choose one of two methods based on if obj
 * inherits from QObject or not.
 *
 */
template <typename OT>
QVariant __SetPropValue(const QMetaProperty &prop, const OT &obj)
{
    return __GetPropValue(prop,
                          obj,
                          typename std::is_base_of<QObject, OT>::type());
}

/// For streaming Dynamic Properties of a Q_GADGET
template <typename OT>
QTextStream &__DumpDynamicProperties(QTextStream & stream, const OT &obj, std::false_type)
{
    Q_UNUSED(obj);

    // Not a QObject, so no dynamic properties
    return stream;
}

/// For streaming Dynamic Properties of a QObject
template <typename OT>
QTextStream &__DumpDynamicProperties(QTextStream & stream, const OT &obj, std::true_type)
{
    // dynamic properties
    QList<QByteArray> propertyNames = obj.dynamicPropertyNames();
    stream << propertyNames.size() << "|";
    foreach (QByteArray name, propertyNames)
    {
        stream << name << obj.property(name).toString() << "|";
    }
    return stream;
}

/**
 * @brief Generic way to stream dynamic properties from Object or Q_GADGET.
 *
 * This uses a type check to choose one of two methods based on if obj
 * inherits from QObject or not.
 *
 */
template <typename OT>
QTextStream &__DumpDynamicProperties(QTextStream & stream, const OT &obj)
{
    return __DumpDynamicProperties(stream, obj, typename std::is_base_of<QObject, OT>::type());
}

/// A generic template for stream the properites of a Q_GADGET or QObject
template <class OT> QString DumpProperties (const OT &obj)
{
    QString string;
    QTextStream stream(&string);
    // static properties
    int propertyCount = obj.staticMetaObject.propertyCount();
    stream << propertyCount << "|";
    for (int i = 0; i < propertyCount; ++i)
    {
        QMetaProperty   prop = obj.staticMetaObject.property(i);
        QVariant        value = __GetPropValue(prop, obj);
        QVariant::Type  type = value.type();
        QString         typeName = value.typeName();
        if (typeName.isEmpty())
        {
            typeName = QString("User-%1").arg(value.userType());
        }
        QString         name = prop.name();
        stream << name << ":(" << typeName << ")";
        if (type == QVariant::StringList)
        {
            QStringList sl = value.toStringList();
            stream << sl.size() << "[" << sl.join(",") << "]";
        }
        else if (type == QVariant::List)
        {
            QList<QVariant> list = value.toList();
            stream << list.size() << "[";
            QStringList substr;
            foreach (QVariant v, list)
            {
                substr << v.toString();
            }
            stream << substr.join(",") << "]";
        }
        else if (type == QVariant::Map)
        {
            QStringList substr;
            QVariantMap map = value.toMap();
            stream << map.size() << "[";
            foreach(QString key, map.keys())
            {
                substr << (key + "-" + map[key].toString());
            }
            stream << substr.join(",") << "]";
        }
        else
        {
            stream << value.toString();
        }
        stream << "|";
    }
    __DumpDynamicProperties(stream, obj);
    return string;
}

/**
 * @brief   Stream a the properties of a QObject to a QDataStream
 *
 * @note    This only works for QObjects!
 *
 * @note    This does not properly output QVariant data.
 */
template <class OT> QDataStream &__InsertIntoStream(QDataStream &stream, const OT &obj, std::true_type)
{
    // static properties
    int propertyCount = obj.metaObject()->propertyCount();
    stream << propertyCount;
    for (int i = 0; i < propertyCount; ++i)
    {
        QMetaProperty   prop = obj.metaObject()->property(i);
        QByteArray name = prop.name();
        QVariant value = prop.read(&obj);
        stream << name << value;
    }
    // dynamic properties
    QList<QByteArray> propertyNames = obj.dynamicPropertyNames();
    stream << propertyNames.size();
    foreach (QByteArray name, propertyNames)
    {
        QVariant value = obj.property(name);
        stream << name << value;
    }
    return stream;
}

/**
 * @brief   Stream the properties of a Q_GADGET to a QDataStream
 *
 * @note    This only works for Q_GADGETs
 *
 * @note    This does not properly output QVariant data.
 */
template <class OT> QDataStream &__InsertIntoStream(QDataStream &stream, const OT &obj, std::false_type)
{
    // static properties
    int propertyCount = obj.staticMetaObject.propertyCount();
    stream << propertyCount;
    for (int i = 0; i < propertyCount; ++i)
    {
        QMetaProperty   prop = obj.staticMetaObject.property(i);
        QByteArray name = prop.name();
        QVariant value = __GetPropValue(prop, obj);
        stream << name << value;
    }
    return stream;
}

/// Stream the properties of a class to a QDataStream
template <class OT> QDataStream &operator<<(QDataStream &stream, const OT &obj)
{
    return __InsertIntoStream(stream, obj, typename std::is_base_of<QObject, OT>::type());
}


/**
 * @brief   Load the properties of a class from a QDataStream
 *
 * @note    This only works for QObjects!
 */
template <class OT> QDataStream &__TakeFromStream(QDataStream &stream, OT &obj, std::true_type)
{
    int propertyCount;
    // static properties
    stream >> propertyCount;
    for (int i = 0; i < propertyCount; ++i)
    {
        QByteArray name;
        QVariant value;
        stream >> name;
        stream >> value;
        obj.setProperty(name, value);
    }
    // dynamic properties
    stream >> propertyCount;
    for (int i = 0; i < propertyCount; ++i)
    {
        QByteArray name;
        QVariant value;
        stream >> name;
        stream >> value;
        obj.setProperty(name, value);
    }
    return stream;
}

/**
 * @brief   Load the properties of a Q_GADGET from a QDataStream
 *
 * @note    This only works for Q_GADGETs
 */
template <typename OT> QDataStream &__TakeFromStream(QDataStream &stream, OT &obj, std::false_type)
{
    int propertyCount;
    // static properties
    stream >> propertyCount;
    for (int i = 0; i < propertyCount; ++i)
    {
        QByteArray name;
        QVariant value;
        stream >> name;
        stream >> value;
        int index = obj.staticMetaObject.indexOfProperty(name);
        if (index != -1)
        {
            QMetaProperty   prop = obj.staticMetaObject.property(i);
            prop.writeOnGadget(&obj, value);
        }
        else
        {
            /// @todo Handle unknown property better
            qDebug() << __FUNCTION__ << "!!! Property name" << name << " not found in class " << typeid(OT).name() << "!!!";
        }
    }
    return stream;
}

template <class OT> QDataStream &operator>>(QDataStream &stream, OT &obj)
{
    return __TakeFromStream(stream, obj, typename std::is_base_of<QObject, OT>::type());
}

/**
 * @brief   Check that all of the properties of two objects of the same type match.
 *
 * @note    This only works for QObjects!
 */
template <class T> bool __ArePropertiesMatched(const T &left, const T &right, std::true_type)
{
    bool result = true;

    int propertyCountL = left.metaObject()->propertyCount();
    int propertyCountR = right.metaObject()->propertyCount();
    QList<QByteArray> propertyNamesL = left.dynamicPropertyNames();
    QList<QByteArray> propertyNamesR = right.dynamicPropertyNames();

    // start by making sure they have the same number of properties.  If not,
    // then it is not a match.
    if ((propertyCountL != propertyCountR) ||
        (propertyNamesL.size() != propertyNamesR.size())
       )
    {
        result = false;
    }
    else
    {
        // check the static properties
        for (int i = 0; i < propertyCountL; ++i)
        {
            QMetaProperty   propL = left.metaObject()->property(i);
            QByteArray name = propL.name();
            QVariant valueL = propL.read(&left);
            QVariant valueR = right.property(name);

            if (valueL.type() != valueR.type())
            {
                result = false;
                break;
            }
            // special case for handling NAN and INF
            if ((valueL.type() == QMetaType::Double) || (valueL.type() == QMetaType::Float))
            {
                double dvl = valueL.toDouble();
                double dvr = valueL.toDouble();
                if (isnan(dvl) && (isnan(dvl) == isnan(dvr)))
                {
                    result = true;
                    break;
                }
                if (isinf(dvl) && (isinf(dvl) == isinf(dvr)))
                {
                    result = true;
                    break;
                }
            }
            // test if the property exists and has a matching value
            if (valueL != right.property(name))
            {
                result = false;
                break;
            }
        }
        if (true == result) // haven't found a failed case yet
        {
            // check the dynamic properties
            foreach (QByteArray name, propertyNamesL)
            {
                if (left.property(name) != right.property(name))
                {
                    result = false;
                    break;
                }
            }
        }
    }

    return result;
}

/**
 * @brief   Check that all of the properties of two objects of the same type match.
 *
 * @note    This only works for Q_GADGETs!
 */
template <class T> bool __ArePropertiesMatched(const T &left, const T &right, std::false_type)
{
    bool result = true;

    int propertyCountL = left.staticMetaObject.propertyCount();
    int propertyCountR = right.staticMetaObject.propertyCount();

    // start by making sure they have the same number of properties.  If not,
    // then it is not a match.
    if (propertyCountL != propertyCountR)
    {
        result = false;
    }
    else
    {
        // check the static properties
        for (int i = 0; i < propertyCountL; ++i)
        {
            QMetaProperty   propR = right.staticMetaObject.property(i);
            QMetaProperty   propL = left.staticMetaObject.property(i);
            QByteArray      nameL = propL.name();
            QByteArray      nameR = propR.name();
            if (nameL != nameR)
            {
                result = false;
                break;
            }
            QVariant valueL = __GetPropValue(propL, left);
            QVariant valueR = __GetPropValue(propR, right);

            if (valueL.type() != valueR.type())
            {
                result = false;
                break;
            }
            // special case for handling NAN and INF
            if ((valueL.type() == QMetaType::Double) || (valueL.type() == QMetaType::Float))
            {
                double dvl = valueL.toDouble();
                double dvr = valueL.toDouble();
                if (isnan(dvl) && (isnan(dvl) == isnan(dvr)))
                {
                    result = true;
                    break;
                }
                if (isinf(dvl) && (isinf(dvl) == isinf(dvr)))
                {
                    result = true;
                    break;
                }
            }
        }
    }

    return result;
}

/**
 * @brief   Check that all of the properties of two objects of the same type match.
 *
 */
template <class T> bool ArePropertiesMatched(const T &left, const T &right)
{
    return __ArePropertiesMatched(left, right, typename std::is_base_of<QObject, T>::type());
}

/**
 * @brief   Copy all of the properties of a QObject from one instance to another.
 *
 * @note    This only works for QObjects!
 *
 * @todo    Copying of enumerated values seems to not work.
 */
template <class T> void __CopyProperties(T &left, const T&right, std::true_type)
{
    if (&left != &right)    // only copy if not the same instance
    {
        int propertyCountL = left.metaObject()->propertyCount();

        // check the static properties
        for (int i = 0; i < propertyCountL; ++i)
        {
            QMetaProperty   propR = right.metaObject()->property(i);
            QVariant    val = propR.read(&right);
            left.setProperty(propR.name(), val);
            //-- qDebug() << "-->O " << i << ":" << propR.name() << " = " << val << val.toString();
        }
    }
}

/**
 * @brief   Copy all of the properties of a Q_GADGET from one instance to another.
 *
 * @note    This only works for Q_GADGETs!
 *
 * @todo    Copying of enumerated values seems to not work.
 */
template <class T> void __CopyProperties(T &left, const T&right, std::false_type)
{
    if (&left != &right)    // only copy if not the same instance
    {
        int propertyCountL = left.staticMetaObject.propertyCount();

        // check the static properties
        for (int i = 0; i < propertyCountL; ++i)
        {
            QMetaProperty   propR = right.staticMetaObject.property(i);
            QMetaProperty   propL = left.staticMetaObject.property(i);
            propL.writeOnGadget(&left, __GetPropValue(propR, right));

            //-- qDebug() << "-->G " << i << ":" << propL.name() << " = " << __GetPropValue(propR, right);
        }
    }
}

/**
 * @brief   Copy all of the properties from one instance to another.
 *
 * @note    This only works for QObjects and Q_GADGETS!
 *
 * @todo    Copying of enumerated values seems to not work.
 */
template <class T> void CopyProperties(T &left, const T &right)
{
    __CopyProperties(left, right, typename std::is_base_of<QObject, T>::type());
}

/**
 * @brief   Return all of the Property names associated with the QObject
 *
 * @param[in]   left - the object to work on
 *
 * @return  A list of all of the property names
 *
 * @note    This works for QObjects
 */
template <class T> QList<QByteArray> __GetPropNames(const T &left, std::true_type)
{
    QList<QByteArray> propNames;
    const QMetaObject *metaobject = left.metaObject();
    int count = metaobject->propertyCount();
    for (int i=0; i<count; ++i)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        propNames.append(name);
    }
    propNames.append(left.dynamicPropertyNames());
    return propNames;
}

/**
 * @brief   Return all of the Property names associated with the Q_GADGET
 *
 * @param[in]   left - the object to work on
 *
 * @return  A list of all of the property names
 *
 * @note    This works for Q_GADGETs
 */
template <class T> QList<QByteArray> __GetPropNames(const T &left, std::false_type)
{
    QList<QByteArray> propNames;
    QMetaObject metaobject = left.staticMetaObject;
    int count = metaobject.propertyCount();
    for (int i=0; i<count; ++i)
    {
        QMetaProperty metaproperty = metaobject.property(i);
        const char *name = metaproperty.name();
        propNames.append(name);
    }
    return propNames;
}

/**
 * @brief   Return all of the Property names associated with the object
 *
 * @param[in]   left - the object to work on
 *
 * @return  A list of all of the property names
 */
template <class T> QList<QByteArray> __GetPropNames(const T &left)
{
    return __GetPropNames(left, typename std::is_base_of<QObject, T>::type());
}

/**
 * @brief   Set the QObject object's name'd property to the value given.
 *
 * @param[out]  object  - the object to set
 * @param[in]   name    - the name of the property
 * @param[in]   value   - the value of the property
 *
 * @note    This only works on QObjects.
 */
template <class T> void __SetProperty(T &object, const char *name, const QVariant &value, std::true_type)
{
    object.setProperty(name, value);
}

/**
 * @brief   Set the Q_GADGET object's name'd property to the value given.
 *
 * @param[out]  object  - the object to set
 * @param[in]   name    - the name of the property
 * @param[in]   value   - the value of the property
 *
 * @note    This only works on Q_GADGETS.
 */
template <class T> void __SetProperty(T &object, const char *name, const QVariant &value, std::false_type)
{
    QMetaObject metaObject = object.staticMetaObject;
    int propIndex = metaObject.indexOfProperty(name);
    if (propIndex != -1)
    {
        QMetaProperty prop = metaObject.property(propIndex);
        prop.writeOnGadget(&object, value);
    }
}

/**
 * @brief   Set the object's name'd property to the value given.
 *
 * @param[out]  object  - the object to set
 * @param[in]   name    - the name of the property
 * @param[in]   value   - the value of the property
 */
template <class T> void __SetProperty(T &object, const char *name, const QVariant &value)
{
    __SetProperty(object, name, value, typename std::is_base_of<QObject, T>::type());
}

/**
 * @brief   Get the QObject object's name'd property to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the property
 *
 * @note    This only works on QObjects.
 */
template <class T> QVariant __GetProperty(T &object, const char *name, std::true_type)
{
    return object.property(name);
}

/**
 * @brief   Get the Q_GADGET object's name'd property to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the property
 *
 * @note    This only works on Q_GADGETS.
 */
template <class T> QVariant __GetProperty(T &object, const char *name, std::false_type)
{
    QVariant    retVal;
    QMetaObject metaObject = object.staticMetaObject;
    int propIndex = metaObject.indexOfProperty(name);
    if (propIndex != -1)
    {
        QMetaProperty prop = metaObject.property(propIndex);
        retVal = prop.readOnGadget(&object);
    }
    return retVal;
}

/**
 * @brief   Get the object's name'd Property to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the PropertyType
 */
template <class T> QVariant __GetProperty(T &object, const char *name)
{
    return __GetProperty(object, name, typename std::is_base_of<QObject, T>::type());
}


/**
 * @brief   Get the QObject object's name'd property type to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the property
 *
 * @note    This only works on QObjects.
 */
template <class T> QVariant::Type __GetPropertyType(T &object, const char *name, std::true_type)
{
    return object.property(name).type();
}

/**
 * @brief   Get the Q_GADGET object's name'd PropertyType to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the PropertyType
 *
 * @note    This only works on Q_GADGETS.
 */
template <class T> QVariant::Type __GetPropertyType(T &object, const char *name, std::false_type)
{
    QVariant::Type    retVal;
    QMetaObject metaObject = object.staticMetaObject;
    int propIndex = metaObject.indexOfProperty(name);
    if (propIndex != -1)
    {
        QMetaProperty prop = metaObject.property(propIndex);
        retVal = prop.type();
    }
    return retVal;
}

/**
 * @brief   Get the object's name'd PropertyType to the value given.
 *
 * @param[out]  object  - the object to Get
 * @param[in]   name    - the name of the PropertyType
 */
template <class T> QVariant::Type __GetPropertyType(T &object, const char *name)
{
    return __GetPropertyType(object, name, typename std::is_base_of<QObject, T>::type());
}

#endif // PROPERTYCLASSTEMPLATES_H
