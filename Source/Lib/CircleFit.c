#include "../main.h"

//--------------------------------------------------------------------
//
// 函数功能  ：圆拟合
//
// 函数参数  ：x: 拟合点的x坐标
//             y: 拟合点的y坐标
//
// 返回值    ：向量存储半径、圆心x坐标和圆心y坐标
//
// 用法示例  ：
//
// 修改记录  ：2015-11-28 老井 jingyanjun@sut.edu.cn
//             原始代码
//
//--------------------------------------------------------------------
gsl_vector*
CircleFit ( gsl_vector *x, gsl_vector *y ) {
	gint len = x->size;
	gsl_vector *result = gsl_vector_alloc ( 3 );
	gfloat xavg = VectorSum(x) / len;
	gfloat yavg = VectorSum(y) / len;

	// 拟合圆心和半径
	gsl_vector *u = gsl_vector_alloc ( len );
	gsl_vector *v = gsl_vector_alloc ( len );
	gsl_vector_memcpy ( u, x );
	gsl_vector_add_constant ( u, -xavg );
	gsl_vector_memcpy ( v, y );
	gsl_vector_add_constant ( v, -yavg );

	gsl_vector *uu = gsl_vector_alloc ( len );
	gsl_vector *uv = gsl_vector_alloc ( len );
	gsl_vector *vv = gsl_vector_alloc ( len );
	gsl_vector *uuu = gsl_vector_alloc ( len );
	gsl_vector *vvv = gsl_vector_alloc ( len );
	gsl_vector *uvv = gsl_vector_alloc ( len );
	gsl_vector *vuu = gsl_vector_alloc ( len );

	gsl_vector_memcpy ( uu, u );
	gsl_vector_mul ( uu, u );
	gsl_vector_memcpy ( vv, v );
	gsl_vector_mul ( vv, v );
	gsl_vector_memcpy ( uv, u );
	gsl_vector_mul ( uv, v );
	gsl_vector_memcpy ( uuu, u );
	gsl_vector_mul ( uuu, uu );
	gsl_vector_memcpy ( vvv, v );
	gsl_vector_mul ( vvv, vv );
	gsl_vector_memcpy ( uvv, u );
	gsl_vector_mul ( uvv, vv );
	gsl_vector_memcpy ( vuu, v );
	gsl_vector_mul ( vuu, uu );

	double Ad[] = { VectorSum(uu), VectorSum(uv),
				   VectorSum(uv), VectorSum(vv) };

	double Bd[] = { (VectorSum(uuu) + VectorSum(uvv))/2,
					(VectorSum(vvv) + VectorSum(vuu))/2 };

	gsl_matrix_view A = gsl_matrix_view_array ( Ad, 2, 2 );
	gsl_vector_view B = gsl_vector_view_array ( Bd, 2 );

	gsl_vector *res = gsl_vector_alloc ( 2 );
	int s;
	gsl_permutation *p = gsl_permutation_alloc ( 2 );
	gsl_linalg_LU_decomp ( &A.matrix, p, &s );
	gsl_linalg_LU_solve ( &A.matrix, p, &B.vector, res );

	gsl_vector_set ( result, 0, sqrt ( gsl_vector_get(res,0) * gsl_vector_get(res,0)
						+ gsl_vector_get(res,1) * gsl_vector_get(res,1)
						+ ( VectorSum(uu) + VectorSum(vv) )/len ) );
	gsl_vector_set ( result, 1, gsl_vector_get(res,0) + xavg );
	gsl_vector_set ( result, 2, gsl_vector_get(res,1) + yavg );

	gsl_vector_free ( res );
	gsl_vector_free ( u );
	gsl_vector_free ( v );
	gsl_vector_free ( uu );
	gsl_vector_free ( uv );
	gsl_vector_free ( vv );
	gsl_vector_free ( uuu );
	gsl_vector_free ( vvv );
	gsl_vector_free ( uvv );
	gsl_vector_free ( vuu );

	return result;
}
