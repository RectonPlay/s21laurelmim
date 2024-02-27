#include "s21_matrix_oop.h"
bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool res = true;
  if (matrix_ == nullptr || other.matrix_ == nullptr)
    throw std::out_of_range("Invalid matrix");
  if (rows_ != other.rows_ || cols_ != other.cols_) res = false;
  for (int i = 0; i < rows_ && res == true; i++)
    for (int j = 0; j < cols_ && res == true; j++)
      if (std::fabs(matrix_[i][j] - other.matrix_[i][j]) > S21Matrix::eps)
        res = false;
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_ || matrix_ == nullptr ||
      other.matrix_ == nullptr)
    throw std::out_of_range("Different matrix dimensions or invalid matrices");
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] += other.matrix_[i][j];
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_ || matrix_ == nullptr ||
      other.matrix_ == nullptr)
    throw std::out_of_range("Different matrix dimensions or invalid matrices");
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] -= other.matrix_[i][j];
}

void S21Matrix::MulNumber(const double num) {
  if (matrix_ == nullptr) throw std::out_of_range("Invalid matrix");
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr || cols_ != other.rows_)
    throw std::out_of_range("Cols != rows or invalid matrices");
  S21Matrix res(rows_, other.cols_);
  for (int i = 0; i < res.rows_; i++)
    for (int j = 0; j < res.cols_; j++) {
      res.matrix_[i][j] = 0;
      for (int k = 0; k < cols_; k++)
        res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
    }
  *this = std::move(res);
}

S21Matrix S21Matrix::Transpose() {
  if (matrix_ == nullptr) throw std::out_of_range("Invalid matrix");
  S21Matrix res(cols_, rows_);
  for (int i = 0; i < res.rows_; i++)
    for (int j = 0; j < res.cols_; j++) res.matrix_[i][j] = matrix_[j][i];
  return res;
}

S21Matrix S21Matrix::CalcComplements() {
  if (rows_ != cols_ || matrix_ == nullptr || rows_ == 0 || cols_ == 0)
    throw std::out_of_range("Matrix is not square or does not exist");
  S21Matrix res(rows_, cols_);
  if (rows_ == 1)
    res(0, 0) = 1;
  else {
    for (int i = 0; i < res.rows_; i++)
      for (int j = 0; j < res.cols_; j++) {
        res(i, j) = pow(-1, i + j) * s21_minor(i, j);
      }
  }
  return res;
}

double S21Matrix::s21_minor(int k, int l) {
  double res = 0.0;
  if (rows_ == cols_ && matrix_ != nullptr && k >= 0 && l >= 0 && k < rows_ &&
      l < cols_) {
    S21Matrix M(rows_ - 1, cols_ - 1);
    int p, q;
    for (int i = 0; i < M.rows_; i++)
      for (int j = 0; j < M.cols_; j++) {
        p = i >= k ? i + 1 : i;
        q = j >= l ? j + 1 : j;
        M(i, j) = matrix_[p][q];
      }
    res = M.Determinant();
  }
  return res;
}

double S21Matrix::Determinant() {
  double res;
  if (rows_ != cols_ || matrix_ == nullptr)
    throw std::out_of_range("Matrix is not square or does not exist");
  S21Matrix temp = *this;
  res = temp.s21_Gauss();
  for (int i = 0; i < rows_; i++) res *= temp(i, i);

  return res;
}

int S21Matrix::s21_Gauss() {
  double sub;
  int iMax, countSwap = 0;
  for (int i = 0; i < rows_; i++) {
    iMax = i;
    for (int k = i + 1; k < rows_; k++)
      if (std::fabs(matrix_[k][i]) > std::fabs(matrix_[iMax][i])) iMax = k;
    if (std::fabs(matrix_[iMax][i]) > S21Matrix::eps) {
      std::swap(matrix_[i], matrix_[iMax]);
      if (i != iMax) countSwap++;
      for (int k = i + 1; k < rows_; k++) {
        sub = matrix_[k][i] / matrix_[i][i];
        for (int j = i; j < cols_; j++) matrix_[k][j] -= sub * matrix_[i][j];
      }
    }
  }
  return countSwap % 2 == 0 ? 1 : -1;
}

S21Matrix S21Matrix::InverseMatrix() {
  if (rows_ != cols_ || matrix_ == nullptr)
    throw std::out_of_range("Matrix is not square or does not exist");
  double det = Determinant();
  if (std::fabs(det) < S21Matrix::eps)
    throw std::out_of_range("Matrix determinant is 0");
  S21Matrix res(rows_, cols_);
  S21Matrix M = CalcComplements();
  M = M.Transpose();
  res = M * (1.0 / det);
  return res;
}