//
// Created by radua on 9/29/2021.
//

#include "value.h"
#include "ValueFactory.h"

std::istream& operator>>(std::istream& is, const Value& value) {
    value.value_ptr->read(is);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    value.value_ptr->write(os);
    return os;
}

void write_to_buffer(const Value& val, char *buffer, std::size_t val_sz) {
    val.value_ptr->write_to_buffer(buffer, val_sz);
}

Value::Value(const std::string& type, std::size_t size) {
    value_ptr = ValueFactory::Instance()->createValue(type);
}

bool operator==(const Value& val1, const Value& val2) {
    return *val1.value_ptr == *val2.value_ptr;
}

void read_from_buffer(Value& value, char *buffer, std::size_t sz) {
    value.value_ptr->read_from_buffer(buffer, sz);
}

bool operator<(const Value& val1, const Value& val2) {
    return *val1.value_ptr < *val2.value_ptr;
}
