//
// Created by radua on 1/13/2022.
//

#ifndef OOP_PROJ_TABLEEXCEPTION_H
#define OOP_PROJ_TABLEEXCEPTION_H

struct TableException : public std::runtime_error {
public:
    explicit TableException(const std::string& str) : std::runtime_error(str) {}
};

struct TableInitException : public TableException {
public:
    explicit TableInitException(const std::string& file_name) :
            TableException("Table initialization exception: Cannot open table file \"" +
                           file_name + "\".") {}
};

struct TableCreateException : public TableException {
public:
    explicit TableCreateException(const std::string& file_name) :
            TableException("Cannot create table: File \"" +
                           file_name + "\" already exists.") {}
};

struct ColumnNotFound : public TableException {
public:
    explicit ColumnNotFound(const std::string& column_name) :
            TableException("Column \"" +
                           column_name + "\" is not part of the Table.") {}
};

#endif //OOP_PROJ_TABLEEXCEPTION_H
