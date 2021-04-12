#ifndef MODEL_H
#define MODEL_H

#include <QtCore>

class Model : public QAbstractTableModel
{
    Q_OBJECT
private:
public:
    explicit Model(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
public:
    class Data {
    public:
        QString username;
        QString password;
    public:
        friend bool operator==(const Data &data1, const Data& data2) {
            return data1.username == data2.username
                    && data1.password == data2.password;
        }
    };
    typedef QVector<Data> DataContainer;
public:
    bool contains(const Data& data);
private:
    DataContainer data_;
};

Q_DECLARE_METATYPE(Model::Data)

#endif // MODEL_H
