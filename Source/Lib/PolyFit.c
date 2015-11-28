#include "../main.h"

//--------------------------------------------------------------------
//
// 函数功能  ：曲线拟合
//
// 函数参数  ：x: 拟合点x坐标；
//             y: 拟合点y坐标；
//             n: 拟合阶次
//
// 返回值    ：拟合后多项式，顺序是a0、a1、a2... an
//
// 用法示例  ：
//
// 修改记录  ：2015-11-28 老井 jingyanjun@sut.edu.cn
//             原始代码
//
//--------------------------------------------------------------------
gsl_vector*
PolyFit ( gsl_vector *x, gsl_vector *y, gint n ) {
	gint len = x->size;
	gsl_vector *res = gsl_vector_alloc ( n+1 );
	gsl_vector *temp = gsl_vector_alloc ( len );
	gsl_vector_memcpy ( temp, y );
	gdouble sumy[n+1];
	for ( int i=0; i<n+1; i++ ) {
		sumy[i] = VectorSum(temp);
		gsl_vector_mul ( temp, x );
	}
	gdouble sumx[2*(n+1) + 1];
	sumx[0] = len;
	gsl_vector_memcpy ( temp, x );
	for ( int i=0; i<2*(n+1); i++ ) {
		sumx[i+1] = VectorSum(temp);
		gsl_vector_mul ( temp, x );
	}

	gsl_matrix *A = gsl_matrix_alloc ( n+1, n+1 );
	for ( int i=0; i<(n+1); i++ ) {
		for ( int j=0; j<(n+1); j++ ) {
			gsl_matrix_set ( A, i, j, sumx[i+j] );
		}
	}
	gsl_vector_view B = gsl_vector_view_array ( sumy, n+1 );

	int s;
	gsl_permutation *p = gsl_permutation_alloc ( n+1 );
	gsl_linalg_LU_decomp ( A, p, &s );
	gsl_linalg_LU_solve ( A, p, &B.vector, res );

	gsl_vector_free ( temp );
	return res;
} 


void TestPolyFit () {
	gsl_vector *x = gsl_vector_alloc ( 6 );
	gsl_vector *y = gsl_vector_alloc ( 6 );
	gsl_vector_set ( x, 0, 0 );
	gsl_vector_set ( x, 1, 1 );
	gsl_vector_set ( x, 2, 2 );
	gsl_vector_set ( x, 3, 3 );
	gsl_vector_set ( x, 4, 4 );
	gsl_vector_set ( x, 5, 5 );
	gsl_vector_set ( y, 0, 2.1 );
	gsl_vector_set ( y, 1, 7.7 );
	gsl_vector_set ( y, 2, 13.6 );
	gsl_vector_set ( y, 3, 27.2 );
	gsl_vector_set ( y, 4, 40.9 );
	gsl_vector_set ( y, 5, 61.1 );

//	gsl_vector *res = PolyFit ( x, y, 3 );
//	gsl_vector *res = PolyFit ( x, y, 2 );
	gsl_vector *res = PolyFit ( x, y, 1 );

	gsl_vector_free ( res );
	gsl_vector_free ( x );
	gsl_vector_free ( y );
}
