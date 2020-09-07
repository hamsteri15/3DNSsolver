
namespace matrix
{

void square_matrix_vector_product(const double *A, const double *x, double *b, int n)			  
{

    for (int i = 0; i < n; i++) {
        b[i] = 0.0;
        for (int j = 0; j < n; j++) {
            b[i] += A[i*n+j] * x[j];
        }
    }
  
}



void square_matrix_matrix_product(const double *A, const double *B, double *C, int n)			  
{
  
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n+j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i*n+j] += A[i*n+k] * B[k*n+j];
            }
        }
    }

}

void square_matrix_matrix_transpose_sum(const double *A, double *C, int n){


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i+n*j] = A[i+n*j] + A[j + n*i];
        }
    }    

}



double square_matrix_trace(const double *A, int n){

    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j){
                sum+=A[i+n*j];
            }
        }
    }
    return sum;        
}


void square_matrix_diagonal_addition(double *A, const double d, int n){

    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j){
                A[i+n*j] += d;
            }
        }
    }
            
}



}//namespace