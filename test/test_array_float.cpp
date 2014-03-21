#include "testsettings.hpp"
#ifdef TEST_ARRAY_FLOAT

#include <tightdb/array_basic.hpp>
#include <tightdb/column.hpp>

#include "util/unit_test.hpp"
#include "util/test_only.hpp"

using namespace tightdb;


namespace {

template<class T, size_t N> inline size_t size_of_array(T(&)[N])
{
    return N;
}

// Article about comparing floats:
// http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

float float_val[] = {
    0.0f,
    1.0f,
    2.12345f,
    12345.12f,
    -12345.12f
};
const size_t float_val_len = size_of_array(float_val);

double double_val[] = {
    0.0,
    1.0,
    2.12345,
    12345.12,
    -12345.12
};
const size_t double_val_len = size_of_array(double_val);

} // anonymous namespace


// TODO: Add test of full range of floats.

template <class C, typename T>
void BasicArray_AddGet(T val[], size_t valLen)
{
    C f;
    for (size_t i=0; i<valLen; ++i) {
        f.add(val[i]);

        CHECK_EQUAL(i+1, f.size());

        for (size_t j=0; j<i; ++j) {
            CHECK_EQUAL(val[j], f.get(j));
        }
    }

    f.clear();
    CHECK_EQUAL(0, f.size());

    f.destroy();    // cleanup
}
TEST(ArrayFloat_AddGet) { BasicArray_AddGet<ArrayFloat, float>(float_val, float_val_len); }
TEST(ArrayDouble_AddGet){ BasicArray_AddGet<ArrayDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicArray_AddManyValues()
{
    C f;
    const size_t repeats = 1100;
    for (size_t i=0; i<repeats; ++i) {
        f.add(T(i));
        T val = f.get(i);
        CHECK_EQUAL(T(i), val);
        CHECK_EQUAL(i+1, f.size());
    }
    for (size_t i=0; i<repeats; ++i) {
        T val = f.get(i);
        CHECK_EQUAL(T(i), val);
    }

    f.clear();
    CHECK_EQUAL(0, f.size());

    f.destroy();    // cleanup
}
TEST(ArrayFloat_AddManyValues) { BasicArray_AddManyValues<ArrayFloat, float>(); }
TEST(ArrayDouble_AddManyValues){ BasicArray_AddManyValues<ArrayDouble, double>(); }

template <class C, typename T>
void BasicArray_Delete()
{
    C f;
    for (size_t i=0; i<5; ++i)
        f.add( T(i) );

    // Delete first
    f.erase(0);
    CHECK_EQUAL(4, f.size());
    CHECK_EQUAL(1, f.get(0));
    CHECK_EQUAL(2, f.get(1));
    CHECK_EQUAL(3, f.get(2));
    CHECK_EQUAL(4, f.get(3));

    // Delete last
    f.erase(3);
    CHECK_EQUAL(3, f.size());
    CHECK_EQUAL(1, f.get(0));
    CHECK_EQUAL(2, f.get(1));
    CHECK_EQUAL(3, f.get(2));

    // Delete middle
    f.erase(1);
    CHECK_EQUAL(2, f.size());
    CHECK_EQUAL(1, f.get(0));
    CHECK_EQUAL(3, f.get(1));

    // Delete all
    f.erase(0);
    CHECK_EQUAL(1, f.size());
    CHECK_EQUAL(3, f.get(0));
    f.erase(0);
    CHECK_EQUAL(0, f.size());
    CHECK(f.is_empty());

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Delete) { BasicArray_Delete<ArrayFloat, float>(); }
TEST(ArrayDouble_Delete){ BasicArray_Delete<ArrayDouble, double>(); }


template <class C, typename T>
void BasicArray_Set(T val[], size_t valLen)
{
    C f;
    CHECK_EQUAL(0, f.size());
    for (size_t i=0; i<valLen; ++i)
        f.add(val[i]);
    CHECK_EQUAL(valLen, f.size());

    f.set(0, T(1.6));
    CHECK_EQUAL(T(1.6), f.get(0));
    f.set(3, T(987.23));
    CHECK_EQUAL(T(987.23), f.get(3));

    CHECK_EQUAL(val[1], f.get(1));
    CHECK_EQUAL(val[2], f.get(2));
    CHECK_EQUAL(val[4], f.get(4));
    CHECK_EQUAL(valLen, f.size());

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Set) { BasicArray_Set<ArrayFloat, float>(float_val, float_val_len); }
TEST(ArrayDouble_Set){ BasicArray_Set<ArrayDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicArray_Insert()
{
    C f;
    const T v0 = T(123.970);
    const T v1 = T(-321.971);
    T v2 = T(555.972);
    T v3 = T(-999.973);

    // Insert in empty array
    f.insert(0, v0);
    CHECK_EQUAL(v0, f.get(0));
    CHECK_EQUAL(1, f.size());

    // Insert in top
    f.insert(0, v1);
    CHECK_EQUAL(v1, f.get(0));
    CHECK_EQUAL(v0, f.get(1));
    CHECK_EQUAL(2, f.size());

    // Insert in middle
    f.insert(1, v2);
    CHECK_EQUAL(v1, f.get(0));
    CHECK_EQUAL(v2, f.get(1));
    CHECK_EQUAL(v0, f.get(2));
    CHECK_EQUAL(3, f.size());

    // Insert at buttom
    f.insert(3, v3);
    CHECK_EQUAL(v1, f.get(0));
    CHECK_EQUAL(v2, f.get(1));
    CHECK_EQUAL(v0, f.get(2));
    CHECK_EQUAL(v3, f.get(3));
    CHECK_EQUAL(4, f.size());

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Insert) { BasicArray_Insert<ArrayFloat, float>(); }
TEST(ArrayDouble_Insert){ BasicArray_Insert<ArrayDouble, double>(); }

#if 0
// sum() is unused by other classes
template <class C, typename T>
void BasicArray_Sum()
{
    C f;

    T values[] = { T(1.1), T(2.2), T(3.3), T(4.4), T(5.5)};
    double sum = 0.0;
    for (size_t i=0; i<5; ++i) {
        f.add(values[i]);
        sum += values[i];
    }
    CHECK_EQUAL(5, f.size());

    // all
    CHECK_EQUAL(sum, f.sum());
    // first
    CHECK_EQUAL(double(values[0]), f.sum(0, 1));
    // last
    CHECK_EQUAL(double(values[4]), f.sum(4, 5));
    // middle range
    CHECK_EQUAL(double(values[2]) + double(values[3]) + double(values[4]), f.sum(2));
    // single middle
    CHECK_EQUAL(double(values[2]), f.sum(2, 3));
    f.destroy();    // cleanup
}
TEST(ArrayFloat_Sum) { BasicArray_Sum<ArrayFloat, float>(); }
TEST(ArrayDouble_Sum){ BasicArray_Sum<ArrayDouble, double>(); }
#endif

template <class C, typename T>
void BasicArray_Minimum()
{
    C f;
    T res = T();

    CHECK_EQUAL(false, f.minimum(res));

    T values[] = { T(1.1), T(2.2), T(-1.0), T(5.5), T(4.4)};
    for (size_t i=0; i<5; ++i) {
        f.add(values[i]);
    }
    CHECK_EQUAL(5, f.size());

    // middle match in all
    CHECK_EQUAL(true, f.minimum(res));
    CHECK_EQUAL(values[2], res);
    // first match
    CHECK_EQUAL(true, f.minimum(res, 0, 2));
    CHECK_EQUAL(values[0], res);
    // middle range, last match
    CHECK_EQUAL(true, f.minimum(res, 1, 3));
    CHECK_EQUAL(values[2], res);
    // single middle
    CHECK_EQUAL(true, f.minimum(res, 3, 4));
    CHECK_EQUAL(values[3], res);
    // first match in range
    CHECK_EQUAL(true, f.minimum(res, 3, size_t(-1)));
    CHECK_EQUAL(values[4], res);

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Minimum) { BasicArray_Minimum<ArrayFloat, float>(); }
TEST(ArrayDouble_Minimum){ BasicArray_Minimum<ArrayDouble, double>(); }


template <class C, typename T>
void BasicArray_Maximum()
{
    C f;
    T res = T();

    CHECK_EQUAL(false, f.maximum(res));

    T values[] = { T(1.1), T(2.2), T(-1.0), T(5.5), T(4.4)};
    for (size_t i=0; i<5; ++i) {
        f.add(values[i]);
    }
    CHECK_EQUAL(5, f.size());

    // middle match in all
    CHECK_EQUAL(true, f.maximum(res));
    CHECK_EQUAL(values[3], res);
    // last match
    CHECK_EQUAL(true, f.maximum(res, 0, 2));
    CHECK_EQUAL(values[1], res);
    // middle range, last match
    CHECK_EQUAL(true, f.maximum(res, 1, 4));
    CHECK_EQUAL(values[3], res);
    // single middle
    CHECK_EQUAL(true, f.maximum(res, 3, 4));
    CHECK_EQUAL(values[3], res);
    // first match in range
    CHECK_EQUAL(true, f.maximum(res, 3, size_t(-1)));
    CHECK_EQUAL(values[3], res);

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Maximum) { BasicArray_Maximum<ArrayFloat, float>(); }
TEST(ArrayDouble_Maximum){ BasicArray_Maximum<ArrayDouble, double>(); }


template <class C, typename T>
void BasicArray_Find()
{
    C f;

    // Empty list
    CHECK_EQUAL(size_t(-1), f.find_first(0));

    // Add some values
    T values[] = { T(1.1), T(2.2), T(-1.0), T(5.5), T(1.1), T(4.4) };
    for (size_t i=0; i<6; ++i)
        f.add(values[i]);

    // Find (full range: start=0, end=-1)
    CHECK_EQUAL(0, f.find_first(T(1.1)));
    CHECK_EQUAL(5, f.find_first(T(4.4)));
    CHECK_EQUAL(2, f.find_first(T(-1.0)));

    // non-existing
    CHECK_EQUAL(size_t(-1), f.find_first(T(0)));

    // various range limitations
    CHECK_EQUAL(1,          f.find_first(T(2.2), 1, 2));    // ok
    CHECK_EQUAL(1,          f.find_first(T(2.2), 1, 3));
    CHECK_EQUAL(5,          f.find_first(T(4.4), 1));       // defaul end=all
    CHECK_EQUAL(size_t(-1), f.find_first(T(2.2), 1, 1));    // start=end
    CHECK_EQUAL(size_t(-1), f.find_first(T(1.1), 1, 4));    // no match .end 1 too little
    CHECK_EQUAL(4,          f.find_first(T(1.1), 1, 5));    // skip first match, end at last match

    // Find all
    Array resArr;
    f.find_all(resArr, T(1.1), 0);
    CHECK_EQUAL(2, resArr.size());
    CHECK_EQUAL(0, resArr.get(0));
    CHECK_EQUAL(4, resArr.get(1));
    // Find all, range limited -> no match
    resArr.clear();
    f.find_all(resArr, T(1.1), 0, 1, 4);
    CHECK_EQUAL(0, resArr.size());
    resArr.destroy();

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Find) { BasicArray_Find<ArrayFloat, float>(); }
TEST(ArrayDouble_Find){ BasicArray_Find<ArrayDouble, double>(); }


template <class C, typename T>
void BasicArray_Count()
{
    C f;

    // Empty list
    CHECK_EQUAL(0, f.count(0));

    // Add some values
    //                0       1        2       3       4       5
    T values[] = { T(1.1), T(2.2), T(-1.0), T(5.5), T(1.1), T(4.4)};
    for (size_t i=0; i<6; ++i) {
        f.add(values[i]);
    }
    // count full range
    CHECK_EQUAL(0, f.count(T(0.0)));
    CHECK_EQUAL(1, f.count(T(4.4)));
    CHECK_EQUAL(1, f.count(T(-1.0)));
    CHECK_EQUAL(2, f.count(T(1.1)));

    // limited range
    CHECK_EQUAL(0, f.count(T(4.4), 0, 5));
    CHECK_EQUAL(1, f.count(T(4.4), 0, 6));
    CHECK_EQUAL(1, f.count(T(4.4), 5, 6));

    CHECK_EQUAL(0, f.count(T(-1.0), 1, 2));
    CHECK_EQUAL(0, f.count(T(-1.0), 3, 4));
    CHECK_EQUAL(1, f.count(T(-1.0), 2, 4));
    CHECK_EQUAL(1, f.count(T(-1.0), 1));

    f.destroy();    // cleanup
}
TEST(ArrayFloat_Count) { BasicArray_Count<ArrayFloat, float>(); }
TEST(ArrayDouble_Count){ BasicArray_Count<ArrayDouble, double>(); }


template <class C, typename T>
void BasicArray_Compare()
{
    C f1, f2;

    // Empty list
    CHECK_EQUAL(true, f1.compare(f2));
    CHECK_EQUAL(true, f2.compare(f1));

    // Add some values
    T values[] = { T(1.1), T(2.2), T(-1.0), T(5.5), T(1.1), T(4.4)};
    for (size_t i=0; i<6; ++i) {
        f1.add(values[i]);
        f2.add(values[i]);
        CHECK_EQUAL(true, f1.compare(f2));
    }

    f1.erase(0);
    CHECK_EQUAL(false, f1.compare(f2));

    f2.erase(0);
    CHECK_EQUAL(true, f1.compare(f2));

    f1.destroy();    // cleanup
    f2.destroy();
}
TEST(ArrayFloat_Compare) { BasicArray_Compare<ArrayFloat, float>(); }
TEST(ArrayDouble_Compare){ BasicArray_Compare<ArrayDouble, double>(); }

#endif // TEST_ARRAY_FLOAT
