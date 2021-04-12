#include "model.h"

Model::Model(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant Model::data(const QModelIndex &index, int role) const
{

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        int row = index.row();
        int col = index.column();
        Data data = data_[row];
        if (col == 0)
            return data.username;
        else if (col == 1)
            return data.password;
    }
    return QVariant();
}

int Model::columnCount(const QModelIndex &) const
{
    return 2;
}

int Model::rowCount(const QModelIndex &) const
{
    return data_.size();
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        Data data = value.value<Data>();
        data_[index.row()] = value.value<Data>();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool Model::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        data_.insert(row, Data());
    }
    endInsertRows();

    return true;
}

bool Model::contains(const Model::Data &data)
{
    return data_.contains(data);
}
