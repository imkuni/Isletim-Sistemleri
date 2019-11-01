package oslabmatrixmultiplication;

public class MatrixMultiplier extends Thread {
    int mat1[][];
    int mat2[][];
    int result[][];
    int row;
    int col;

    public MatrixMultiplier(){}

    public MatrixMultiplier(int[][] mat1, int[][] mat2, int[][] result, int row, int col) {
        super();
        this.mat1 = mat1;
        this.mat2 = mat2;
        this.result = result;
        this.row = row;
        this.col = col;
    }

    @Override
    public void run() {
        int res = 0;
        for(int i = 0; i < mat1[row].length; i++){
            res += mat1[row][i] * mat2[i][col];
        }

        result[row][col] = res;
    }
}
