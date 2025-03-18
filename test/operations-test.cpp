#include "matrix.h"
#include "test-helpers.h"

#include <gtest/gtest.h>

#include <numeric>

namespace ct::test {

class OperationsTest : public ::testing::Test {
protected:
  void SetUp() override {
    Element::reset_allocations();
  }
};

TEST_F(OperationsTest, element_access) {
  Matrix<Element> a(2, 3);

  a(0, 2) = 5;
  a(1, 1) = 42;
  a(1, 2) = a(0, 2);

  const Matrix<Element>& ca = a;

  EXPECT_EQ(0, ca(0, 0));
  EXPECT_EQ(0, ca(0, 1));
  EXPECT_EQ(5, ca(0, 2));
  EXPECT_EQ(0, ca(1, 0));
  EXPECT_EQ(42, ca(1, 1));
  EXPECT_EQ(5, ca(1, 2));
}

TEST_F(OperationsTest, data) {
  constexpr size_t ROWS = 40;
  constexpr size_t COLS = 100;

  Matrix<Element> a(ROWS, COLS);
  fill(a);

  {
    Element* data = a.data();
    for (size_t i = 0; i < ROWS; ++i) {
      for (size_t j = 0; j < COLS; ++j) {
        EXPECT_EQ(elem(i, j), data[i * COLS + j]);
      }
    }
  }

  {
    const Element* data = std::as_const(a).data();
    for (size_t i = 0; i < ROWS; ++i) {
      for (size_t j = 0; j < COLS; ++j) {
        EXPECT_EQ(elem(i, j), data[i * COLS + j]);
      }
    }
  }
}

TEST_F(OperationsTest, range_based_for) {
  constexpr size_t ROWS = 40;
  constexpr size_t COLS = 100;

  Matrix<Element> a(ROWS, COLS);
  fill(a);

  for (size_t i = 0; Element x : a) {
    EXPECT_EQ(a(i / COLS, i % COLS), x);
    ++i;
  }

  for (size_t i = 0; Element x : std::as_const(a)) {
    EXPECT_EQ(a(i / COLS, i % COLS), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : a) {
    EXPECT_EQ(a.data() + i, &x);
    EXPECT_EQ(a(i / COLS, i % COLS), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : std::as_const(a)) {
    EXPECT_EQ(a.data() + i, &x);
    EXPECT_EQ(a(i / COLS, i % COLS), x);
    ++i;
  }

  for (size_t i = 0; Element & x : a) {
    EXPECT_EQ(a.data() + i, &x);
    EXPECT_EQ(a(i / COLS, i % COLS), x);
    x += 2;
    ++i;
  }

  for (size_t i = 0; Element x : a) {
    EXPECT_EQ(elem(i / COLS, i % COLS) + 2, x);
    ++i;
  }
}

namespace {

template <class It>
class View {
public:
  View(It begin, It end)
      : _begin(begin)
      , _end(end) {}

  It begin() const {
    return _begin;
  }

  It end() const {
    return _end;
  }

private:
  It _begin;
  It _end;
};

} // namespace

TEST_F(OperationsTest, row_range_based_for) {
  constexpr size_t ROWS = 40;
  constexpr size_t COLS = 100;
  constexpr size_t CHOSEN_ROW = 2;

  Matrix<Element> a(ROWS, COLS);
  fill(a);

  View row_view(a.row_begin(CHOSEN_ROW), a.row_end(CHOSEN_ROW));
  View const_row_view(std::as_const(a).row_begin(CHOSEN_ROW), std::as_const(a).row_end(CHOSEN_ROW));

  for (size_t i = 0; Element x : row_view) {
    EXPECT_EQ(a(CHOSEN_ROW, i), x);
    ++i;
  }

  for (size_t i = 0; Element x : const_row_view) {
    EXPECT_EQ(a(CHOSEN_ROW, i), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : row_view) {
    EXPECT_EQ(a.data() + COLS * CHOSEN_ROW + i, &x);
    EXPECT_EQ(a(CHOSEN_ROW, i), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : const_row_view) {
    EXPECT_EQ(a.data() + COLS * CHOSEN_ROW + i, &x);
    EXPECT_EQ(a(CHOSEN_ROW, i), x);
    ++i;
  }

  for (size_t i = 0; Element & x : row_view) {
    EXPECT_EQ(a.data() + COLS * CHOSEN_ROW + i, &x);
    EXPECT_EQ(a(CHOSEN_ROW, i), x);
    x += 2;
    ++i;
  }

  for (size_t i = 0; Element x : a) {
    size_t row = i / COLS;
    size_t col = i % COLS;
    if (row == CHOSEN_ROW) {
      EXPECT_EQ(elem(row, col) + 2, x);
    } else {
      EXPECT_EQ(elem(row, col), x);
    }
    ++i;
  }
}

TEST_F(OperationsTest, col_range_based_for) {
  constexpr size_t ROWS = 40;
  constexpr size_t COLS = 100;
  constexpr size_t CHOSEN_COL = 2;

  Matrix<Element> a(ROWS, COLS);
  fill(a);

  View col_view(a.col_begin(CHOSEN_COL), a.col_end(CHOSEN_COL));
  View const_col_view(std::as_const(a).col_begin(CHOSEN_COL), std::as_const(a).col_end(CHOSEN_COL));

  for (size_t i = 0; Element x : col_view) {
    EXPECT_EQ(a(i, CHOSEN_COL), x);
    ++i;
  }

  for (size_t i = 0; Element x : const_col_view) {
    EXPECT_EQ(a(i, CHOSEN_COL), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : col_view) {
    EXPECT_EQ(a.data() + CHOSEN_COL + COLS * i, &x);
    EXPECT_EQ(a(i, CHOSEN_COL), x);
    ++i;
  }

  for (size_t i = 0; const Element& x : const_col_view) {
    EXPECT_EQ(a.data() + CHOSEN_COL + COLS * i, &x);
    EXPECT_EQ(a(i, CHOSEN_COL), x);
    ++i;
  }

  for (size_t i = 0; Element & x : col_view) {
    EXPECT_EQ(a.data() + CHOSEN_COL + COLS * i, &x);
    EXPECT_EQ(a(i, CHOSEN_COL), x);
    x += 2;
    ++i;
  }

  for (size_t i = 0; Element x : a) {
    size_t row = i / COLS;
    size_t col = i % COLS;
    if (col == CHOSEN_COL) {
      EXPECT_EQ(elem(row, col) + 2, x);
    } else {
      EXPECT_EQ(elem(row, col), x);
    }
    ++i;
  }
}

TEST_F(OperationsTest, compare) {
  Matrix<Element> a({
      {10, 20, 30},
      {40, 50, 60},
  });
  Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });
  Matrix<Element> c({
      {10, 20, 30},
      {42, 50, 60},
  });
  Matrix<Element> d({
      {10, 20},
      {30, 40},
      {50, 60},
  });
  Matrix<Element> e({
      {10, 20, 30},
  });
  Matrix<Element> f({
      {{10}},
      {{40}},
  });

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  EXPECT_TRUE(a != c);
  EXPECT_FALSE(a == c);

  EXPECT_TRUE(a != d);
  EXPECT_FALSE(a == d);

  EXPECT_TRUE(a != e);
  EXPECT_FALSE(a == e);

  EXPECT_TRUE(a != f);
  EXPECT_FALSE(a == f);
}

TEST_F(OperationsTest, compare_empty) {
  Matrix<Element> a, b;
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  expect_allocations(0);
}

TEST_F(OperationsTest, add) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });
  const Matrix<Element> c({
      {11, 22, 33},
      {44, 55, 66},
  });

  size_t expected_allocations = a.size() + b.size() + c.size();

  expect_equal(c, a + b);

  expect_allocations(expected_allocations += c.size() * 2);

  a += b;
  expect_equal(c, a);

  expect_allocations(expected_allocations);
}

TEST_F(OperationsTest, add_return_value) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });
  const Matrix<Element> c({
      {21, 42, 63},
      {84, 105, 126},
  });

  (a += b) += b;
  expect_equal(c, a);

  expect_allocations(a.size() + b.size() + c.size());
}

TEST_F(OperationsTest, sub) {
  Matrix<Element> a({
      {11, 22, 33},
      {44, 55, 66},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });
  const Matrix<Element> c({
      {1, 2, 3},
      {4, 5, 6},
  });

  size_t expected_allocations = a.size() + b.size() + c.size();

  expect_equal(c, a - b);

  expect_allocations(expected_allocations += c.size() * 2);

  a -= b;
  expect_equal(c, a);

  expect_allocations(expected_allocations);
}

TEST_F(OperationsTest, sub_return_value) {
  Matrix<Element> a({
      {21, 42, 63},
      {84, 105, 126},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });
  const Matrix<Element> c({
      {1, 2, 3},
      {4, 5, 6},
  });

  (a -= b) -= b;
  expect_equal(c, a);

  expect_allocations(a.size() + b.size() + c.size());
}

TEST_F(OperationsTest, mul) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  });
  const Matrix<Element> b({
      {10, 40},
      {20, 50},
      {30, 60},
  });
  const Matrix<Element> c({
      {140, 320},
      {320, 770},
      {500, 1220},
  });

  size_t expected_allocations = a.size() + b.size() + c.size();

  expect_equal(c, a * b);
  expect_allocations(expected_allocations += c.size());

  a *= b;
  expect_equal(c, a);

  expect_allocations(expected_allocations += c.size());
}

TEST_F(OperationsTest, mul_return_value) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  });
  const Matrix<Element> b({
      {10, 40},
      {20, 50},
      {30, 60},
  });
  const Matrix<Element> c({
      {150, 360},
      {340, 820},
      {530, 1280},
  });

  size_t expected_allocations = a.size() + b.size() + c.size();

  (a *= b) += b;
  expect_equal(c, a);

  expect_allocations(expected_allocations += c.size());
}

TEST_F(OperationsTest, mul_scalar) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });

  size_t expected_allocations = a.size() + b.size();

  expect_equal(b, a * 10);
  expect_equal(b, 10 * a);

  expect_allocations(expected_allocations += b.size() * 4);

  a *= 10;
  expect_equal(b, a);

  expect_allocations(expected_allocations);
}

TEST_F(OperationsTest, mul_scalar_return_value) {
  Matrix<Element> a({
      {1, 2, 3},
      {4, 5, 6},
  });
  const Matrix<Element> b({
      {10, 20, 30},
      {40, 50, 60},
  });

  (a *= 5) *= 2;
  expect_equal(b, a);

  expect_allocations(a.size() + b.size());
}

} // namespace ct::test
