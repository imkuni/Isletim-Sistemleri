package oslabmatrixmultiplication;

import java.util.ArrayList;
import java.util.List;

public class App {

    static int M = 400, G = 400, N = 400;
    static int mat1[][] = new int[M][G];
    static int mat2[][] = new int[G][N];
    
    static int result[][] = new int[M][N];
    static Thread  threadArray [] =new Thread [M*N];
    
    
  

    public static void main(String[] args) throws InterruptedException {


        for(int i = 0; i < M; i++){
            for(int j = 0; j < G; j++){
                mat1[i][j] = (int)(Math.random() * 10);
            }
        }

        for(int i = 0; i < G; i++){
            for(int j = 0; j < N; j++){
                mat2[i][j] = (int)(Math.random() * 10);
            }
        }

        long beg1=System.nanoTime();
          multiplyV1();
        long end1=System.nanoTime();
        
       

        System.out.println("Sequential Product V1 time:"+(end1-beg1));
       // printMatrix();

        //Threadleri başlat.
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                threadArray[i*M+j] = new MatrixMultiplier(mat1, mat2, result, i, j);
                //threads.add(thread);
                threadArray[i*M+j].start();
            }
        }
        //Threadleri bekle.
        long beg2=System.nanoTime();
         for(int i=0; i<M*N; i++)
           threadArray[i].join();
       long end2=System.nanoTime();
        System.out.println("Threaded product time :"+(end2-beg2));
       // printMatrix();
    }

    static void printMatrix(){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                System.out.print(result[i][j] + " ");
            }
            System.out.println();
        }
    }
    

    static void multiplyV1(){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                for(int k = 0; k < G; k++){
                    result[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
    }
    
    
   
}
