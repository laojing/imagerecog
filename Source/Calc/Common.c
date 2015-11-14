#include "../main.h"

void
DispVector ( gsl_vector *v ) {
	for ( int i=0; i<v->size; i++ ) {
		printf ( "%7.4f\t", gsl_vector_get ( v, i ) );
	}
	printf ( "\n" );
	//fflush ( stdout );
}

void
DispMatrix ( gsl_matrix *m ) {
	for ( int i=0; i<m->size1; i++ ) {
		for ( int j=0; j<m->size2; j++ ) {
			printf ( "%10.2f\t", gsl_matrix_get ( m, i, j ) );
		}
		printf ( "\n" );
		printf ( "\n" );
	}
	printf ( "\n" );
	printf ( "\n" );
	printf ( "\n" );
	//fflush ( stdout );
}

float
MatrixSum ( gsl_matrix *x ) {
	float sum = 0;
	for ( int i=0; i<x->size1; i++ ) {
		for ( int j=0; j<x->size2; j++ ) {
			sum += gsl_matrix_get ( x, i, j );
		}
	}
	return sum;
}

gsl_matrix*
Convolution ( gsl_matrix *A, gsl_matrix *B ) {
	gsl_matrix *res = gsl_matrix_alloc ( A->size1, A->size2 );
	int brow = (B->size1 - 1)/2;
	int bcol = (B->size2 - 1)/2;
	gsl_matrix *temp = gsl_matrix_alloc ( B->size1, B->size2 );
	for ( int i=0; i<A->size1; i++ ) {
		for ( int j=0; j<A->size2; j++ ) {
			for ( int ii=-brow; ii<=brow; ii++ ) {
				for ( int jj=-bcol; jj<=bcol; jj++ ) {
					if (   ii + i >= 0 && ii + i < A->size1
						&& jj + j >= 0 && jj + j < A->size2 )
						gsl_matrix_set ( temp, ii+brow, jj + bcol, 
								gsl_matrix_get ( A, ii+i, jj+j ) );
					else 
						gsl_matrix_set ( temp, ii+brow, jj + bcol, 0 );
				}
			}
			gsl_matrix_mul_elements ( temp, B );
			gsl_matrix_set ( res, i, j, MatrixSum(temp) );
		}
	}
	gsl_matrix_free ( temp );
	return res;
}

void
DestroyVectorList ( gpointer v ) {
 	gsl_vector_free ( (gsl_vector*)v );
}
