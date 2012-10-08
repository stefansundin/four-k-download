/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/


#ifndef COMPONENTMODEL_XMLSERIALIZABLE_H
#define COMPONENTMODEL_XMLSERIALIZABLE_H

#include <QtXml>

namespace ComponentModel
{

class IXmlSerializable
{
public:
    virtual bool serialize(QDomDocument& doc, QDomElement& element) const = 0;
    virtual bool deserialize(const QDomDocument& doc, const QDomElement& element) = 0;
};

} // ComponentModel

#endif // COMPONENTMODEL_XMLSERIALIZABLE_H
