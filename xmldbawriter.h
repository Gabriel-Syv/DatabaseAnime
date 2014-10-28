#ifndef XMLDBAWRITER_H
#define XMLDBAWRITER_H

#include "globalenum.h"
#include <QIODevice>
#include <QXmlStreamWriter>
#include <QMap>

class XmlDbaWriter
{
    QXmlStreamWriter    _streamWriter;
    sections::section   _currentSection;

    QString getSectionName(sections::section section);

public:
    XmlDbaWriter(QIODevice* stream);
    bool writeNext(QMap<QString, QString> data);

    bool hasError()const;
    sections::section getCurrentSection()const;
    void setSection(sections::section section);

    void startSection(sections::section section);
    void startSection();
    void endSection();

    void startElement(QString name);
    void endElement();

    void startDocument();
    void endDocument();
};

#endif // XMLDBAWRITER_H