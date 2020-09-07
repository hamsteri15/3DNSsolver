#ifndef matrix_hh
#define matrix_hh



namespace matrix
{

////////////////////////////////////////////////////////////
///
///    	\brief 			Square matrix times vector Ax = b
///		
///		\param A		Input square matrix 
///	   	\param x		Input vector
///	   	\param b		Output vector
///	   	\param n		Length of vector			
///
////////////////////////////////////////////////////////////
void square_matrix_vector_product(const double *A, const double *x, double *b, int n);

////////////////////////////////////////////////////////////
///
///    	\brief 			Square matrix times matrix AB = C
///		
///		\param A		Input square matrix (left)
///		\param B		Input square matrix (right)
///	   	\param C		Output square matrix
///	   	\param n		Number of columns=rows			
///
////////////////////////////////////////////////////////////
void square_matrix_matrix_product(const double *A, const double *B, double *C, int n);

////////////////////////////////////////////////////////////
///
///    	\brief 			Square matrix plus matrix transpose A + A^T = C
///		
///		\param A		Input square matrix 
///	   	\param C		Output square matrix
///	   	\param n		Number of columns=rows			
///
////////////////////////////////////////////////////////////
void square_matrix_matrix_transpose_sum(const double *A, double *C, int n);


////////////////////////////////////////////////////////////
///
///    	\brief 			Square matrix trace
///		
///		\param A		Input square matrix 
///	   	\param n		Number of columns=rows			
///
////////////////////////////////////////////////////////////
double square_matrix_trace(const double *A, int n);

////////////////////////////////////////////////////////////
///
///    	\brief 			Square matrix trace
///		
///		\param A		Input square matrix 
///	   	\param n		Number of columns=rows			
///
////////////////////////////////////////////////////////////
double square_matrix_diagonal_addition(double *A, const double d, int n);

}


#endif
