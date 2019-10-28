/* Federal University of Maranhao
 * Author: FELIX, Eder M. S.
 * Date: June 10, 2019. Sao Luis, MA, Brazil
 * Description: This program peforms the multiplication
 * between two matrices of gmp floats.
 * Given the two matrices A[m,n] and B[k,m] the user enters
 * with the following parameters:
 * Input:
 *	m = number os lines of A
 *	n = number of colums of A
 *	l = number of lines of B
 *	c = number of colums of B
 *	a = value of all elements of A
 *	b = value of all elements of B
 *	nthr = number of threads to be used
 * the result must be saved in a text file in the following
 * format:
 * c11;c12;c13/c21;c22;c23/c31;c32;c33
 * That is, colums are separated by semicolons and lines are
 * separated by slashes.
 *
 */
/*
 * Includes
 */
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
/*
 * Defines
 */

/*
 * Matrix struct
 */
struct matrix{
	mpf_t** mat;
	unsigned int n_rows;
	unsigned int n_cols;
};
/*
 * Matrix data struct
 */
struct params_data{
	struct matrix* matA;
	struct matrix* matB;
	struct matrix* matC;
	struct matrix* matD;
};

/*
 * Init matrix function
 */
void init_mpf_matrix(struct matrix* _matdata,int rows, int cols,int value){
	
	mpf_t** matrix = _matdata->mat;
	_matdata->n_rows = rows;
	_matdata->n_cols = cols;

	/* Init the matrix elements with value */
	for(int i = 0; i<rows; i++){
		for(int j=0; j<cols; j++){
			mpf_init(matrix[i][j]);
			mpf_set_ui(matrix[i][j], value);

		}
	}

}

/*
 * Init gmp float matrix elements FUNCTION
 */
struct params_data* init_matrix_data(int m, int n, int l, int c, int a, int b, int op){
	struct params_data* _pdata;
	struct matrix* _matA;
	struct matrix* _matB;
	struct matrix* _matC;
	struct matrix* _matD;

	_pdata = (struct params_data*)malloc(sizeof(struct params_data));
	
	_matA = (struct matrix*)malloc(sizeof(struct matrix));
	_matB = (struct matrix*)malloc(sizeof(struct matrix));

	/* Alloc space for the matrices */
	_matA->mat = (mpf_t**)malloc(m*sizeof(mpf_t*));
	for(int i=0; i<m; i++)
		*((_matA->mat)+i) = (mpf_t*)malloc(n*sizeof(mpf_t));

	_matB->mat = (mpf_t**)malloc(l*sizeof(mpf_t*));
	for(int i=0; i<l; i++)
		*((_matB->mat)+i) = (mpf_t*)malloc(c*sizeof(mpf_t));


	/* MULTIPLICATION IS ENABLED */
	if((op==2)||(op==3)){
		_matC = (struct matrix*)malloc(sizeof(struct matrix));
		_matC->mat = (mpf_t**)malloc(m*sizeof(mpf_t*));
		for(int i=0; i<m; i++)
			*((_matC->mat)+i) = (mpf_t*)malloc(c*sizeof(mpf_t));
		init_mpf_matrix(_matC, m, c, 0);
		_pdata->matC = _matC;
	}

	/* SUM IS ENABLE */
	
	if((op==1)||(op==3)){
		_matD = (struct matrix*)malloc(sizeof(struct matrix));
		_matD->mat = (mpf_t**)malloc(m*sizeof(mpf_t*));
		for(int i=0; i<m; i++)
			*((_matD->mat)+i) = (mpf_t*)malloc(n*sizeof(mpf_t));
		init_mpf_matrix(_matD, m, n, 0);
		_pdata->matD = _matD;
	}

	/* Init the matrices */
	
	init_mpf_matrix(_matA, m, n, a);
	init_mpf_matrix(_matB, l, c, b);
	
	/* Passing the values to the params struct */
	_pdata->matA = _matA;
	_pdata->matB = _matB;
	
	
	return _pdata;
}

/*
 * add operation FUNCTION
 */
void operation_add(struct params_data* _pdata){
	/* Retrive the params */
	
	struct matrix* _matdataA = _pdata->matA;
     	struct matrix* _matdataB = _pdata->matB;
     	struct matrix* _matdataD = _pdata->matD;

	mpf_t** _matA = _matdataA->mat;
	mpf_t** _matB = _matdataB->mat;
	mpf_t** _matD = _matdataD->mat;

	const int _rows = _matdataD->n_rows;
	const int _cols = _matdataD->n_cols;
	

	for(int i = 0;i < _rows ; i++){
        	for(int j = 0; j < _cols ; j++){
			mpf_add(_matD[i][j], _matA[i][j], _matB[i][j]);
          	}
     	}
}

/*
 * Mult operation FUNCTION
 */
void operation_mult(struct params_data* _pdata){
	/* Retrieve the params */
	
	struct matrix* _matdataA = _pdata->matA;
     	struct matrix* _matdataB = _pdata->matB;
     	struct matrix* _matdataC = _pdata->matC;

	mpf_t** _matA = _matdataA->mat;
	mpf_t** _matB = _matdataB->mat;
	mpf_t** _matC = _matdataC->mat;

	const int _rows = _matdataC->n_rows;
	const int _cols = _matdataC->n_cols;
	const int _n = _matdataA->n_cols;

	mpf_t sum;
	mpf_t mult;
	mpf_init(sum);
	mpf_init(mult);

	for(int i = 0; i < _rows ; i++){
        	for(int j = 0; j < _cols ; j++){
			//_matC[i][j] = 0;
			mpf_set_ui(_matC[i][j],0);
			for(int k=0; k<_n; k++){
               			//_matC[i][j] += (_matA[i][k] * _matB[k][j]);
				mpf_mul(mult, _matA[i][k], _matB[k][j]);
				mpf_set(sum, _matC[i][j]);
				mpf_add(_matC[i][j], sum, mult);
			}
          	}
     	}

}
/*
 * Write gmp matrix to file FUNCTION
 */
void write_to_file(struct matrix* _matdata, FILE* _stream){

	mpf_t** _mat = _matdata->mat;
	const int _rows = _matdata->n_rows;
	const int _cols = _matdata->n_cols;

	for(int i=0; i<_rows; i++){
		for(int j=0; j<_cols; j++){
			gmp_fprintf(_stream, "%.2Ff", _mat[i][j]);

			if(j==(_cols-1)) gmp_fprintf(_stream, "/ ");
			else gmp_fprintf(_stream,"; ");
		}
	}


}
/*
 * Define operations
 *
 * Meaning of the returned value:
 * 0: none operation is supported
 * 1: only sum is supported
 * 2: only multiplication is supported
 * 3: all the operations are sopported
 */
int def_operations(int m, int n, int l, int c){
	int sum_valid = 1;
	int mult_valid = 1;
	
	if(m<=0||n<=0||c<=0||l<=0) return 0;

	if(n!=l) mult_valid = 0;
	if((m!=l)||(n!=c)) sum_valid = 0;

	if(mult_valid&&sum_valid) return 3;
	if(mult_valid) return 2;
	if(sum_valid) return 1;
	return 0;
}
/*
 * Print matrix on console
 */
void print_matrix_on_console(struct matrix* _mat_data){
	mpf_t** _mat = _mat_data->mat;
	const int _rows = _mat_data->n_rows;
	const int _cols = _mat_data->n_cols;

	for(int i=0; i<_rows; i++){
		for(int j=0; j<_cols; j++){
			gmp_printf("%.2Ff ", _mat[i][j]);

		}
		gmp_printf("\n");
	}

}
/*
 * Main FUNCTION
 */
void main(int argc, char* argv[]){
	/*
	 * The parameters
	 */
	char* filename_add = "sum_matrix.txt";
	char* filename_mult = "prod_matrix.txt";
	FILE* stream_add; 
	FILE* stream_mult;
	int m,n,l,c,a,b, op;
	struct params_data* pdata;

	/*
	 * Retrieve params - user interface
	 */
	printf("WELCOME! This program performs the following operations:\n");
	printf("A[m,n]*B[l,c] = C[m,c]\n");
	printf("A[m,n] + B[l,c] = C[m,n]\n");
	printf("Enter with the parameters...\n");
	printf("m: ");
	scanf("%d",&m); fflush(stdin);
	printf("n: ");
	scanf("%d",&n); fflush(stdin);
	printf("l: ");
	scanf("%d",&l); fflush(stdin);
	printf("c: ");
	scanf("%d",&c); fflush(stdin);
	printf("Value to fill A: ");
	scanf("%d",&a); fflush(stdin);
	printf("Value to fill B: ");
	scanf("%d",&b); fflush(stdin);

	op = def_operations(m,n,l,c);
	if(op==0){
		printf("Invalid arguments!\n");
		exit(1);
	}
	if(op==1){
		printf("[MAIN]: Multiplication not enabled!\n");
	}
	if(op==2){
		printf("[MAIN]: Sum not enabled!\n");
	}

	/*
      	 * Init params
      	 */
	printf("[Main] Creating data... ");
	fflush(stdout);
	
	pdata = init_matrix_data(m, n, l, c, a, b, op);
	printf("OK\n");

	/*
	 * Performing the operations
	 */
	printf("[MAIN] Performing the operations...");
	fflush(stdout);
	if((op==1)||(op==3)) operation_add(pdata);
	if((op==2)||(op==3)) operation_mult(pdata);
	printf("DONE\n");

	/*
	 * write the results to files
	 */
	printf("[MAIN] Writing the results to files...");
	if((op==1)||(op==3)){
		stream_add = fopen(filename_add, "w");
		write_to_file(pdata->matD, stream_add);
		fclose(stream_add);
	}

	if((op==2)||(op==3)){
		stream_mult = fopen(filename_mult, "w");	
		write_to_file(pdata->matC, stream_mult);
		fclose(stream_mult);
	}
	printf("OK\n");
	

}
