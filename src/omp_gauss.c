/**************************************************
****GAUSS ELIMINATION WITHOUT PARTIAL PIVOTING*****
**************************************************/
#include<stdio.h>
/*******
 Function that performs Gauss-Elimination and returns the Upper triangular matrix:
There are two options to do this in C.
1. Pass a matrix (a) as the parameter, and calculate and store the upperTriangular(Gauss-Eliminated Matrix) in it.
2. Use malloc and make the function of pointer type and return the pointer.
This program uses the first option.
********/

void gaussElimination(int m, int n, double a[m][n]){
    int i,j,k;
    for(i=0; i<m-1; i++){
    #pragma omp parallel for shared(a) private(k,j)
        for(k=i+1; k<m; k++){
            double term=a[k][i]/a[i][i];
            for(j=0; j<n; j++)
                a[k][j]= a[k][j]-term*a[i][j];
        }
    }
}
/*******
Function that reads the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void readMatrix(int m, int n, double matrix[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            scanf("%lf",&matrix[i][j]);
        }
    } 
}
/*******
Function that prints the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void printMatrix(int m, int n, double matrix[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%lf\t",matrix[i][j]);
        }
        printf("\n");
    } 
}
/*******
Function that copies the elements of a matrix to another matrix
Parameters: rows(m),columns(n),matrix1[m][n] , matrix2[m][n]
*******/
void copyMatrix(int m, int n, double matrix1[m][n], double matrix2[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            matrix2[i][j]=matrix1[i][j];
        }
    } 
}
 
int main(){
    int m,n;
    printf("Enter the size of the matrix:\nNo. of rows (m)\n");
    scanf("%d",&m);
    printf("No.of columns (n)\n");
    scanf("%d",&n);
    //Declare a matrix to store the user given matrix
    double a[m][n];
    //Declare another matrix to store the resultant matrix obtained after Gauss Elimination
    double U[m][n];
    printf("\nEnter the elements of matrix:\n");
    readMatrix(m,n,a);
    copyMatrix(m,n,a,U);
    //Perform Gauss Elimination 
    gaussElimination(m,n,U);
    printf("\nThe Upper Triangular matrix after Gauss Eliminiation is:\n\n");
    printMatrix(m,n,U);
}
