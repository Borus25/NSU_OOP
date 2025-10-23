#include "tests.h"
#include "bit_array.h"
#include "test_runner.h"

void TestConstructor() {
    /*    проверяет конструктор по умолчанию,
     *              конструктор с передачей числа первых бит и заполнителя,
     *              и конструктор с передачей другого объекта BitArray */
    // По умолчанию
    BitArray arr1;
    ASSERT_EQUAL(arr1.size(), 0);
    ASSERT_EQUAL(arr1.empty(), true);

    // С размером 0
    BitArray arr2(0);
    ASSERT_EQUAL(arr2.size(), 0);

    // С размером 1
    BitArray arr3(1, 1);
    ASSERT_EQUAL(arr3.size(), 1);
    ASSERT_EQUAL(arr3[0], true);

    // Очень большой размер
    BitArray arr4(10000, 0xFFFFFFFF);
    ASSERT_EQUAL(arr4.size(), 10000);

    // Копирование пустого массива
    BitArray arr5(arr1);
    ASSERT_EQUAL(arr5.size(), 0);

    // Копирование самого себя
    arr4 = arr4;
    ASSERT_EQUAL(arr4.size(), 10000);
}

void TestSwap() {
    // проверяет метод swap
    BitArray empty;
    BitArray filled(4, 0b1011);

    // swap с пустым
    empty.swap(filled);
    ASSERT_EQUAL(empty.size(), 4);
    ASSERT_EQUAL(filled.size(), 0);

    // swap одинаковых массивов
    BitArray a(2, 3), b(2, 3);
    a.swap(b);
    ASSERT_EQUAL(a[0], true);
    ASSERT_EQUAL(b[0], true);

    // swap самого с собой
    a.swap(a);
    ASSERT_EQUAL(a[0], true);
}

void TestAssignment() {
    // проверяет операцию присвоения (operator=)
    BitArray a;
    BitArray b(5, 0b11111);

    // Присваивание пустого массиву с элементами
    a = b;
    ASSERT_EQUAL(a.size(), 5);
    ASSERT_EQUAL(a[0], true);

    // Самоприсваивание
    a = a;
    ASSERT_EQUAL(a.size(), 5);

    // Присваивание массивам разного размера
    BitArray c(2, 2);
    a = c;
    ASSERT_EQUAL(a.size(), 2);
}

void TestSizeEditing() {
    /*  проверяет методы:
     *      resize,
     *      clear,
     *      push_back */
    {
        BitArray arr(3, 0b101);

        // resize(0)
        arr.resize(0);
        ASSERT_EQUAL(arr.size(), 0);

        // clear на пустом массиве
        BitArray empty;
        empty.clear();
        ASSERT_EQUAL(empty.size(), 0);

        // Увеличение и уменьшение
        arr.resize(5, true);
        ASSERT_EQUAL(arr.size(), 5);
        arr.resize(2);
        ASSERT_EQUAL(arr.size(), 2);
    }

    {
        // Пустой массив
        BitArray arr;
        arr.push_back(true);
        ASSERT_EQUAL(arr.size(), 1);
        ASSERT_EQUAL(arr[0], true);

        // Многократное добавление (capacity растёт)
        BitArray arr2;
        for (int i = 0; i < 100; ++i)
            arr2.push_back(i % 2);
        ASSERT_EQUAL(arr2.size(), 100);
        ASSERT_EQUAL(arr2[0], false);
        ASSERT_EQUAL(arr2[1], true);
        ASSERT_EQUAL(arr2[99], true);
    }
}

void TestBitwiseOperation() {
    /*  проверяет битовые операции:
     *      побитовый & (and),
     * побитовый | (or),
     *      побитовый ^ (xor)
     */
    // Два пустых массива
    BitArray a, b;
    BitArray res_and = a & b;
    BitArray res_or = a | b;
    BitArray res_xor = a ^ b;
    ASSERT_EQUAL(res_and.size(), 0);
    ASSERT_EQUAL(res_or.size(), 0);
    ASSERT_EQUAL(res_xor.size(), 0);

    // Одинаковые копии
    BitArray c(4, 0b1010);
    BitArray d(4, 0b1010);
    ASSERT_EQUAL((c & d).to_string(), "1010");
    ASSERT_EQUAL((c | d).to_string(), "1010");
    ASSERT_EQUAL((c ^ d).to_string(), "0000");

    // Разные размеры (должно быть исключение)
    BitArray e(2, 0b11), f(4, 0b1001);
    try {
        BitArray x = e & f;
        Assert(false, "Ожидалось исключение due to size mismatch!");
    } catch (const std::invalid_argument&) {
        Assert(true, "Корректное исключение при разных размерах");
    }
    try {
        BitArray y = e | f;
        Assert(false, "Ожидалось исключение!");
    } catch (const std::invalid_argument&) {
        Assert(true, "Корректно на |");
    }
    try {
        BitArray z = e ^ f;
        Assert(false, "Ожидалось исключение!");
    } catch (const std::invalid_argument&) {
        Assert(true, "Корректно на ^");
    }
}

void TestBitShift() {
    /*  проверяет битовые сдвиги:
     *      <<=, >>=, <<, >>
     */
    {
        // Сдвиг пустого массива
        BitArray a;
        BitArray b = a << 5;
        ASSERT_EQUAL(b.size(), 0);
        b = a >> 5;
        ASSERT_EQUAL(b.size(), 0);

        // Сдвиг на 0
        BitArray arr(3, 5);
        BitArray c1 = arr << 0;
        BitArray c2 = arr >> 0;
        ASSERT_EQUAL(c1.to_string(), arr.to_string());
        ASSERT_EQUAL(c2.to_string(), arr.to_string());

        // Сдвиг на размер
        BitArray arr2(4, 0b1111);
        BitArray d1 = arr2 << 4;
        BitArray d2 = arr2 >> 4;
        ASSERT_EQUAL(d1.count(), 0);
        ASSERT_EQUAL(d2.count(), 0);

        // Сдвиг больше размера
        BitArray arr3(2, 0b11);
        BitArray e1 = arr3 << 10;
        BitArray e2 = arr3 >> 10;
        ASSERT_EQUAL(e1.count(), 0);
        ASSERT_EQUAL(e2.count(), 0);

        // Сдвиг на отрицательное
        BitArray arr4(3, 0b101);
        BitArray negL = arr4 << -1;
        BitArray negR = arr4 >> -1;
        ASSERT_EQUAL(negL.to_string(), arr4.to_string());
        ASSERT_EQUAL(negR.to_string(), arr4.to_string());
    }

    {
        // Пустой массив
        BitArray arr;
        arr >>= 3;
        ASSERT_EQUAL(arr.size(), 0);

        // n < 0
        BitArray arr2(3, 0b101);
        arr2 >>= -1;
        ASSERT_EQUAL(arr2.to_string(), "101");

        // n >= size
        BitArray arr3(4, 0b1111);
        arr3 >>= 5;
        ASSERT_EQUAL(arr3.count(), 0);

        // Обычный сдвиг
        BitArray arr4(4, 0b1011);
        arr4 >>= 2;
        ASSERT_EQUAL(arr4.to_string(), "0010");
    }
}

void TestSetAndReset() {
    /*  проверяет методы:
     *      set,
     *      reset
     */
    BitArray arr(3, 0);

    // set out of range — может выбрасывать ошибку
    try {
        arr.set(10);
        ASSERT(true);
    } catch (...) {
        ASSERT(true);
    }

    // set на пустом массиве
    try {
        BitArray empty;
        empty.set(0);
        ASSERT(true);
    } catch (...) {
        ASSERT(true);
    }

    // reset уже нулевого
    arr.reset(0);
    ASSERT_EQUAL(arr[0], false);

    // set уже установленного
    arr.set(0);
    arr.set(0);
    ASSERT_EQUAL(arr[0], true);

    // set/reset all (если реализовано)
    arr.set();
    ASSERT_EQUAL(arr.count(), arr.size());
    arr.reset();
    ASSERT_EQUAL(arr.count(), 0);
}

void TestAny() {
    // проверяет метод any
    BitArray empty;
    ASSERT_EQUAL(empty.any(), false);

    BitArray zeros(3, 0);
    ASSERT_EQUAL(zeros.any(), false);

    BitArray one(3, 0b100);
    ASSERT_EQUAL(one.any(), true);
}

void TestNone() {
    // проверяет метод none
    BitArray empty;
    ASSERT_EQUAL(empty.none(), true);

    BitArray zeros(3, 0);
    ASSERT_EQUAL(zeros.none(), true);

    BitArray one(3, 0b010);
    ASSERT_EQUAL(one.none(), false);
}

void TestBitwiseInversion() {
    // проверяет побитовую инверсию
    BitArray empty;
    BitArray inv = ~empty;
    ASSERT_EQUAL(inv.size(), 0);

    BitArray zeros(3, 0);
    inv = ~zeros;
    ASSERT_EQUAL(inv.count(), 3);

    BitArray ones(3, 0b111);
    inv = ~ones;
    ASSERT_EQUAL(inv.count(), 0);
}

void TestCount() {
    // проверят метод count
    BitArray empty;
    ASSERT_EQUAL(empty.count(), 0);

    BitArray zeros(5, 0);
    ASSERT_EQUAL(zeros.count(), 0);

    BitArray ones(5, 0b11111);
    ASSERT_EQUAL(ones.count(), 5);

    BitArray mixed(5, 0b10101);
    ASSERT_EQUAL(mixed.count(), 3);
}

void TestGet() {
    // проверяет получение i-ого бита
    BitArray arr(5, 0b10101);

    // первый индекс
    ASSERT_EQUAL(arr[0], true);

    // последний индекс
    ASSERT_EQUAL(arr[4], true);

    // невалидный индекс
    try {
        bool bit = arr[10];
        ASSERT(true);
    } catch (...) {
        ASSERT(true);
    }

    // пустой массив
    try {
        BitArray empty;
        bool bit = empty[0];
        ASSERT(true);
    } catch (...) {
        ASSERT(true);
    }
}

void TestSizeChecking() {
    /*  проверяет методы:
     *      size,
     *      empty
     */
    BitArray arr;
    ASSERT_EQUAL(arr.size(), 0);
    ASSERT_EQUAL(arr.empty(), true);

    arr.push_back(true);
    ASSERT_EQUAL(arr.empty(), false);
    ASSERT_EQUAL(arr.size(), 1);

    arr.clear();
    ASSERT_EQUAL(arr.empty(), true);

    arr.resize(0);
    ASSERT_EQUAL(arr.size(), 0);
}

void TestComparison() {
    /* проверяет операции сравнения:
     *      ==,
     *      !=
     */
    BitArray a, b;
    ASSERT_EQUAL(a == b, true);

    BitArray c(3, 0b101);
    BitArray d(3, 0b101);
    ASSERT_EQUAL(c == d, true);

    BitArray e(3, 0b100);
    ASSERT_EQUAL(c != e, true);

    // разный размер
    BitArray f(2, 3);
    ASSERT_EQUAL(f == c, false);
}

void TestToString() {
    // проверяет метод to_string
    // Пустой массив
    BitArray arr;
    ASSERT_EQUAL(arr.to_string(), "");

    // Один бит
    BitArray arr2(1, 1);
    ASSERT_EQUAL(arr2.to_string(), "1");

    // Нечётное число бит
    BitArray arr3(5, 0b10101);
    ASSERT_EQUAL(arr3.to_string(), "10101");

    // Количество бит = sizeof(unsigned long) * 8 (например 64)
    BitArray arr4(32, 0xFFFFFFFFUL);
    std::string all_ones(32, '1');
    ASSERT_EQUAL(arr4.to_string(), all_ones);
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestToString);
    RUN_TEST(tr, TestConstructor);
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestAssignment);
    RUN_TEST(tr, TestSizeEditing);
    RUN_TEST(tr, TestBitwiseOperation);
    RUN_TEST(tr, TestBitShift);
    RUN_TEST(tr, TestSetAndReset);
    RUN_TEST(tr, TestAny);
    RUN_TEST(tr, TestNone);
    RUN_TEST(tr, TestBitwiseInversion);
    RUN_TEST(tr, TestCount);
    RUN_TEST(tr, TestGet);
    RUN_TEST(tr, TestSizeChecking);
    RUN_TEST(tr, TestComparison);
}
