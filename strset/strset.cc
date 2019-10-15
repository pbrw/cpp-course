// Author: Piotr Borowski
// Important notice: In this project we were not allowed to create own
//					 structers or classes!


#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cstdarg>

#include "strsetconst.h"
#include "strset.h"

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif
    
namespace jnp1 {

using Strset = std::set<std::string>;
using Strsets_container = std::vector<Strset>;
using Ids_occupation = std::vector<bool>;
using Strset_id = unsigned long;

namespace {
enum Command_type {NEW, DELETE, INSERT, REMOVE, TEST, SIZE, CLEAR, COMP};

unsigned long strsets_number;

std::string get_command_name(Command_type type) {
    static std::string names[] = 
    {"new", "delete", "insert", "remove", "test", "size", "clear", "comp"};
    
    return names[type];
}

// Meyer's singletons to prevent static initialization order problem
Strsets_container& strsets_container() {
    static Strsets_container strsets_container;
    return strsets_container;
}

Ids_occupation& ids_occupation() {
    static Ids_occupation ids_occupation;
    return ids_occupation;
}

void initialize_stream() {
    std::ios_base::Init();
}

bool strset_exist(unsigned long id) {
    return strsets_number >= id && ids_occupation()[id];
}

// prints on diagnostic output name of the command and values    of its arguments
void print_info(Command_type type, ...) {
    if (!debug) return;

    va_list args;
    va_start(args, type);
    
    std::string info = "strset_" + get_command_name(type) + "(";
    const char* value;
    
    switch (type) {
        case DELETE:
        case SIZE: 
        case CLEAR: 
            info += std::to_string(va_arg(args, Strset_id));
            break;
        case INSERT:
        case REMOVE:
        case TEST:
            info += std::to_string(va_arg(args, Strset_id)) + ", ";
            value = va_arg(args, const char*);
            info += (value == nullptr ? "NULL" : "\"" + std::string(value) + "\"");
            break;
        case COMP:
            info += std::to_string(va_arg(args, Strset_id)) + ", ";
            info += std::to_string(va_arg(args, Strset_id));
            break;
        case NEW:
            break;
    }
    info += ")";
    
    va_end(args);

    initialize_stream();
    std::cerr << info << std::endl;
}

// pritnts on diagnostic output outcome of the command
void print_result(Command_type type, ...) {
    if (!debug) return;
    
    va_list args;
    va_start(args, type);
    
    std::string info = "strset_" + get_command_name(type) + ": ";

    switch (type) {
        case NEW: 
            info += "set " + std::to_string(va_arg(args, int)) + " created";
            break;
        case DELETE: 
            info += "set " + std::to_string(va_arg(args, int)) + " deleted";
            break;
        case INSERT:
            info += "element \"" + std::string(va_arg(args, char*)) + "\" ";
            info += (va_arg(args, int) ? "inserted" : "was already present");
            break;
        case REMOVE: 
            info += "element " + std::string(va_arg(args, char*)) + " ";
            info += (va_arg(args, int) ? "removed" : "was not present");
            break;
        case SIZE:
            info += "set " + std::to_string(va_arg(args, Strset_id)) + " contains ";
            info += std::to_string(va_arg(args, int)) + " element(s)";
            break;
        case TEST:
            info += "set " + std::to_string(va_arg(args, int)) + " ";
            info += (va_arg(args, int) ? "contains" : "does not contain");
            info += " the element \"" + std::string(va_arg(args, char*)) + "\"";
            break;
        case CLEAR:
            info += "set " + std::to_string(va_arg(args, Strset_id)) + " cleared";
            break;
        case COMP:
            info += "result of comparing set " + std::to_string(va_arg(args, int));
            info += " to set " + std::to_string(va_arg(args, int));
            info += " is " + std::to_string(va_arg(args, int));
            break;
    }
    
    va_end(args);
    
    initialize_stream();
    std::cerr << info << std::endl;
}

// checks if id is valid and prints error if necessary 
bool check_id(Command_type type, Strset_id id) {
    bool result = strset_exist(id);

    if (!result && debug) {
        initialize_stream();
        std::string error = "strset_" + get_command_name(type) + ": set " 
                            + std::to_string(id) + " does not exist";
        std::cerr << error << std::endl;
    }

    return result;
}

// checks if value is valid and prints error if necessary
bool check_value(Command_type type, const char* value) {
    bool result = value != nullptr;

    if (!result && debug) {
        initialize_stream();
        std::string error = "strset_" + get_command_name(type)
                        + ": invalid value (NULL)";
        std::cerr << error << std::endl;
    } 
    return result;
}

// checks if the given set is The 42 Set and prints error if necessary
bool check_set42(Command_type type, Strset_id id) {
    if (id == jnp1::strset42()) {
        if (debug) {
            std::string error = "strset_" + get_command_name(type) + ": attempt to ";
            
            switch (type) {
				case INSERT:
					error += "insert into the 42 set";
					break;
				case REMOVE:
					error += "remove element from the 42 set";
					break;
				case CLEAR:
					error += "clear the 42 set";
					break;
				case DELETE:
					error += "delete the 42 set";
					break;
				default:
					break;
            }
            
            initialize_stream();
            std::cerr << error << std::endl;
        }

        return true;
    }

    return false;
}
} // end of anonynous namespace

unsigned long strset_new() {
    print_info(NEW);

    ++strsets_number;
    strsets_container().resize(strsets_number + 1);
    ids_occupation().resize(strsets_number + 1);

    ids_occupation().back() = 1;

    print_result(NEW, strsets_number);
    return strsets_number;
}

void strset_delete(unsigned long id) {
    print_info(DELETE, id);

    bool is_query_correct = check_id(DELETE, id) && !check_set42(DELETE, id);

    if (is_query_correct) {
        ids_occupation()[id] = false;
        print_result(DELETE, 1);
    }
}

size_t strset_size(unsigned long id) {
    print_info(SIZE, id);

    int result = 0;
    if (strset_exist(id)) result = strsets_container()[id].size();

    print_result(SIZE, id, result);

    return result;
}

void strset_insert(unsigned long id, const char* value) {
    print_info(INSERT, id, value);
    bool is_query_correct = check_value(INSERT, value) && check_id(INSERT, id);
    if (!is_query_correct) return;

    if (strsets_container()[id].empty()) {
        strsets_container()[id].insert(std::string(value));
    }
    else {
        if (!check_set42(INSERT, id)) {
            strsets_container()[id].insert(std::string(value));
        }
        else return;
    }

    print_result(INSERT, value, 1);
}

int strset_test(unsigned long id, const char* value) {
    print_info(TEST, id, value);

    bool is_query_correct = check_id(TEST, id) && check_value(TEST, value);
    if (!is_query_correct) return 0;

    int result = strsets_container()[id].count(std::string(value));
    print_result(TEST, id, result, value);
    
    return result;
}

void strset_remove(unsigned long id, const char* value) {
    print_info(REMOVE, id, value);

    bool is_query_correct = check_value(REMOVE, value) &&
                            check_id(REMOVE, id) &&
                            !check_set42(REMOVE, id);

    if (!is_query_correct) return;

    bool result = false;

    if (strset_test(id, value)) {
        result = true;
        strsets_container()[id].erase(std::string(value));
    }

    print_result(REMOVE, value, result);
}

void strset_clear(unsigned long id) {
    print_info(CLEAR, id);

    bool is_query_correct = check_id(CLEAR, id) && !check_set42(CLEAR, id);
    if (!is_query_correct) return;

    strsets_container()[id].clear();

    print_result(CLEAR, id, 1);
}

int strset_comp(unsigned long id1, unsigned long id2) {
    print_info(COMP, id1, id2);

    Strset strset1, strset2;

    if (strset_exist(id1)) strset1 = strsets_container()[id1];
    if (strset_exist(id2)) strset2 = strsets_container()[id2];

    int result = 0;
    if (strset1 < strset2) result = -1;
    else if (strset1 > strset2) result = 1;
    
    print_result(COMP, id1, id2, result);
    check_id(COMP, id1);
    check_id(COMP, id2);

    return result;
}

} // namespace jnp1
