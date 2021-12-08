//
// Created by radua on 9/29/2021.
//

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include "pager.h"

using std::vector;
using std::string;
using std::sort;

/*
 * Citeste doar headerul fisierului tabelului (daca are cheie primara si pozitia ei)
 */
Pager::Pager(const string& file_name): file_name(file_name),
                                            page_data{0}, row_size(0) {
    std::cout << file_name << std::endl;
    std::fstream file(file_name,
                      std::ios::binary |
                      std::ios::in);

    page_pos = 0;

    file.read(page_data, page_size);

    char *iter = page_data;
    has_primary_key = *(bool *)iter;
    iter += sizeof(has_primary_key);

    primary_key_pos = *(int *)iter;
    iter += sizeof(primary_key_pos);

    file.close();
}

/*
 * Citeste datele despre coloane din fisierul tabelului.
 */
vector<string> Pager::get_column_data() {
    std::cout << page_data << std::endl;
    vector<string> column_data;

    char* iter1 = page_data + HEADER_SIZE;
    char* iter2 = strchr(iter1, '\0');

    while (iter2 != nullptr && iter2-page_data <= page_size) {
        if (*iter1 == 0)
            break;

        string column_name = string(iter1, iter2);
        column_data.emplace_back(iter1, iter2);

        // move iterator to first char after column n
        iter1 = iter2+1;

        // read column type
        string column_type = string(iter1, iter1+1);

        // move iterator to next byte
        iter1++;
        iter2 = iter1;

        column_data.push_back(column_type);
        int column_size;

        if (column_type == "c") {
            column_size = *(int *)iter1;

            iter2 += sizeof(int);
            iter1 = iter2;
        }
        else if (column_type == "i")
            column_size = sizeof(int);
        else
            column_size = sizeof(float);

        column_data.push_back(std::to_string(column_size));

        // get next column n
        iter2 = strchr(iter1, '\0');
    }

    *(int *)page_data = page_size;

    return column_data;
}

/*
 * Incarca in bufferul pagerului urmatoarea pagina, daca exista.
 * Returneaza false daca pagerul se afla deja la ultima pagina din fisier
 */
bool Pager::read_next_page() {
    std::fstream file(file_name,
                      std::ios::binary |
                      std::ios::in);

    page_pos += page_size;

    file.seekg(page_pos);

    if (file.read(page_data, page_size)) {
        file.close();
        return true;
    }

    page_pos -= page_size;
    file.close();

    return false;
}

/*
 * Citeste o anumita pagina din fisier. Paginile sunt numerotate de la 0 la n-1,
 * unde n este numarul curent de pagini din fisier.
 */
bool Pager::read_page_at_pos(int pos) {
    std::fstream file(file_name,
                      std::ios::binary |
                      std::ios::in);

    std::clog << "Reading page at pos " << page_pos << std::endl;

    file.seekg(pos);

    if (pos >= num_valid_pages*page_size)
        return false;

    file.read(page_data, page_size);

    return true;
}

/*
 * Scrie o linie in bufferul pagerului
 */
bool insert_row_in_page(char* page_data, const char* row_data, std::size_t row_size) {
    int first_bit = *(int *)page_data;

    if (first_bit + row_size <= page_size) {
        std::clog << "can insert row\n";
        memcpy(page_data+first_bit, row_data, row_size);
        *(int *)page_data = first_bit + row_size;
        return true;
    }
    return false;
}

/*
 * Scrie datele din bufferul pagerului in pagina corespunzatoare din fisier
 */
void write_page(const string& file_name,
                const char* page_data, std::streampos page_pos) {
    std::fstream file(file_name,
                      std::ios::binary |
                      std::ios::out |
                      std::ios::in);

    std::clog << "Writing page at pos " << page_pos << std::endl;
    file.seekg(page_pos);

    file.write(page_data, page_size);

    file.close();
}

/*
 * Adauga o pagina goala la finalul fisierului
 */
void append_page(const string& file_name) {
    std::fstream file(file_name,
                      std::ios::binary |
                      std::ios::out |
                      std::ios::app);

    file.seekg(0, std::fstream::end);

    std::clog << "Writing new page at position " << file.tellg() << std::endl;

    char empty_page[page_size] = {0};
    *(int *)empty_page = sizeof(int);
    std::clog << "New page with first row going at: " << *(int *)empty_page << std::endl;
    file.write(empty_page, page_size);

    file.close();
}

/*
 * Creeaza linia (as putea imbunatati creand o clasa pentru linii)
 * Si cauta prima pagina unde o poate introduce (prima pagina unde este suficient loc)
 */
void Pager::insert_row(std::istream& repl) {
    std::vector<Value> row;
    char row_data[row_size];
    memset(row_data, 0, row_size);

    char *iter = row_data;
    for (int i = 0; i < column_names.size(); i++) {
        Value val(column_types[i], column_sizes[i]);
        repl >> val;

        write_to_buffer(val, iter, column_sizes[i]);
        iter += column_sizes[i];

        row.push_back(val);
    }

    if (has_primary_key) {
        // todo: implementez btree
    }

    while (!insert_row_in_page(page_data, row_data, row_size)) {
        if (!read_next_page()) {
            std::clog << "Could not fit row in existing page, creating new one\n";
            append_page(file_name);
            read_next_page();
            insert_row_in_page(page_data, row_data, row_size);
            break;
        }
    }

    std::clog << "Writing at page_pos: " << page_pos << std::endl;

    write_page(file_name, page_data, page_pos);
}

vector<vector<Value>> Pager::select_rows_from_page(int pos, const Value& val) {
    vector<vector<Value>> ret;

    vector<vector<Value>> page_rows = get_page_rows();

    for (auto& row : page_rows)
        if (row[pos] == val) ret.push_back(row);

    return ret;
}

vector<vector<Value>> Pager::select_rows(int column_position, const Value& val) {
    vector<vector<Value>> selected_rows;

    if (has_primary_key) {
        // todo: implementez btree
    }

    while (read_next_page()) {
        vector<vector<Value>> new_selected_rows =
                select_rows_from_page(column_position, val);

        std::copy(new_selected_rows.begin(), new_selected_rows.end(),
                  std::back_inserter(selected_rows));
    }

    return selected_rows;
}

/*
 * Returneaza pagina curenta, aflata in bufferul pagerului in forma procesata
 */
std::vector<std::vector<Value>> Pager::get_page_rows() {
    std::vector<std::vector<Value>> ret;

    std::size_t iter = sizeof(int);
    std::size_t end = *(int *)page_data;

    for (iter = sizeof(int); iter < end; ) {
        std::vector<Value> row;
        for (int i = 0; i < column_names.size(); i++) {
            Value val(column_types[i], column_sizes[i]);
            read_from_buffer(val, page_data+iter, column_sizes[i]);
            row.push_back(val);

            iter += column_sizes[i];
        }

        ret.push_back(row);
    }

    return ret;
}

/*
 * Sterg toate liniile care indeplinesc conditia coloana = val din bufferul pagerului
 */
void Pager::delete_rows_from_page(int pos, const Value& val) {
    vector<int> deleted_pos;

    vector<vector<Value>> page_rows = get_page_rows();

    for (int i = 0; i < page_rows.size(); i++) {
        if (page_rows[i][pos] == val) deleted_pos.push_back(i);
    }

    sort(deleted_pos.begin(), deleted_pos.end(), std::greater<>());

    // mut toate liniile dupa cea stearsa cu o pozitie mai aproape de inceputul fisierului
    for (auto& i : deleted_pos) {
        std::clog << "Deleting row at position " << i << std::endl;
        memcpy(page_data+sizeof(int)+row_size*i,
               page_data+*(int *)page_data-row_size,
               row_size);

        *(int *)page_data -= (int)row_size;
    }

    // sterg datele ramase despre linia stearsa
    std::size_t iter = *(int *)page_data;
    memset(page_data+iter, 0, row_size*deleted_pos.size());

    if (!deleted_pos.empty())
        write_page(file_name, page_data, page_pos);
}

void Pager::delete_rows(int column_position, const Value& val) {
    if (has_primary_key) {
        // TODO: implementez btree
    }
    while (read_next_page()) {
        delete_rows_from_page(column_position, val);
    }
}

void Pager::update_rows_from_page(int pos, const Value& old_val, int updated_pos,
                                  const Value& new_val) {
    bool updated_rows = false;

    vector<vector<Value>> page_rows = get_page_rows();

    for (int i = 0; i < page_rows.size(); i++) {
        if (page_rows[i][pos] == old_val) {
            updated_rows = true;

            std::size_t pos_in_row = 0;
            for (int j = 0; j < updated_pos; j++) pos_in_row += column_sizes[j];

            std::size_t pos_in_page = sizeof(int) + i*row_size + pos_in_row;
            write_to_buffer(new_val, page_data+pos_in_page, column_sizes[pos]);
        }
    }

    if (updated_rows)
        write_page(file_name, page_data, page_pos);
}

void Pager::update_rows(int column_position, const Value& old_value,
                        int updated_pos, const Value& new_value) {
    if (has_primary_key) {
        // TODO: implementez btree
    }

    while (read_next_page()) {
        update_rows_from_page(column_position, old_value, updated_pos, new_value);
    }
}
