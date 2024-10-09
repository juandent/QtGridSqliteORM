module;

#include <sqlite_orm/sqlite_orm.h>
#include <cassert>
#include <string>
#include <iostream>
#include <chrono>
#include <format>
#include <sstream>
#include <regex>


export module fixed_decimal_binding;

import fixed_point;

#define ENABLE_THIS_EXAMPLE
#ifdef ENABLE_THIS_EXAMPLE


export {
    ///////////////////////////////
    /// Money binding as real
    /// ///////////////////////////

    /**
    *  This is a helper function
    */

    //  also we need transform functions to make string from Money
    std::string moneyToString(Money money) {
        auto r = std::format("{:.2f}", money.getAsLongDouble());
        return r;
    }

    /**
    *  This is a helper function not used in binding
    */
    Money moneyFromString(const std::string& s) {
        using namespace std::literals;
        using namespace std::chrono;

        std::string naked;
        // remove non digit chars
        for(int i = 0; i < s.length(); ++i) {
            char c = s[i];
            if(c != ',' && c != '$' && c != '�') {
                naked += c;
            }
        }

        long double value = std::stold(naked);
        Money val{value};
        return val;
    }

    /**
 *  This is where magic happens. To tell sqlite_orm how to act
 *  with Money we have to create a few service classes
 *  specializations (traits) in sqlite_orm namespace.
 */
    namespace sqlite_orm {

        /**
     *  First of all is a type_printer template class specialization
     *  It is responsible for sqlite type real representation.
     *  We want Money to be `REAL` so let's just derive from
     *  real_printer.
     */
        template<>
        struct type_printer<Money> : public real_printer {};

        /**
     *  This is a binder class. It is used to bind c++ values to sqlite queries.
     *  Here we have to create Money floating representation and bind it as double
     *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
     *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
     *  More here https://www.sqlite.org/c3ref/bind_blob.html
     */
        template<>
        struct statement_binder<Money> {

            int bind(sqlite3_stmt* stmt, int index, const Money& value) const {
                return statement_binder<double>().bind(stmt, index, value.getAsLongDouble());
            }
        };

        /**
     *  field_printer is used in `dump` and `where` functions. Here we have to create
     *  a string from mapped object.
     */
        template<>
        struct field_printer<Money> {
            std::string operator()(const Money& t) const {
                return moneyToString(t);
            }
        };

        /**
     *  This is a reverse operation: here we have to specify a way to transform string received from
     *  database to our Money object. Here we call `moneyFromString` and throw `std::runtime_error` if it returns null.
     *  Every `row_extractor` specialization must have `extract(const char*)`, `extract(sqlite3_stmt *stmt, int columnIndex)`
     *	and `extract(sqlite3_value* value)` functions which cast to a typed value.
     */
        template<>
        struct row_extractor<Money> {
            Money extract(double value) const {
                Money mny{value};
                return mny;
            }

            Money extract(sqlite3_stmt* stmt, int columnIndex) const {
                double val = sqlite3_column_double(stmt, columnIndex);
                return this->extract(val);
            }
        };
    }

    ////////////////////////////////
    /// end Money binding as TEXT
    ////////////////////////////////

    using namespace sqlite_orm;
    using std::cout;
    using std::endl;

    struct Person {
        int id;
        std::string name;
        Money salary;
    };
#endif

    int test() {
#ifdef ENABLE_THIS_EXAMPLE
        const std::string db_name = "Money.sqlite";
        ::remove(db_name.c_str());

        auto storage = make_storage(db_name,
                                    make_table("Persons",
                                               make_column("id", &Person::id, primary_key().autoincrement()),
                                               make_column("name", &Person::name),
                                               make_column("salary", &Person::salary)));

        using namespace std::chrono;

        Money salary{34.45};

        storage.sync_schema();

        Person person{1, "Juan Dent", salary};  // we are using the implicit operator from year_month_day to Money
        storage.replace(person);

        auto pers = storage.get<Person>(1);
        auto res = pers.salary;  // using the implicit operator from Money to year_month_day
        long double val = res.getAsLongDouble();
        auto str = moneyToString(res);

#endif
        return 0;
    }
}
