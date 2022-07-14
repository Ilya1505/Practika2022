#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QTableWidgetItem>

class TableItem : public QTableWidgetItem {
public:
    TableItem(QString text);
    bool operator <(const QTableWidgetItem &second) const
    {
        return text().toDouble() < second.text().toDouble();
    }
};

#endif // TABLEITEM_H
