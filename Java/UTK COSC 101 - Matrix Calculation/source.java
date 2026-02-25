

import java.util.Scanner;
class MatrixCalc {
    private final static int VECTOR_SIZE = 3;
    private final static int MATRIX_SIZE = VECTOR_SIZE * VECTOR_SIZE;
    public static void main(String[] args) {
        //this is declaring the two arrays for the matrix and the vector, both set to their respective sizes.
        double[] matrix = new double[MATRIX_SIZE];
        double[] vector = new double[VECTOR_SIZE];
        Scanner s = new Scanner(System.in);
        System.out.format("Enter %d matrix values: ", MATRIX_SIZE);
        for (int i = 0; i < MATRIX_SIZE; i++){
            double value = s.nextDouble();
            matrix[i] = value;
        }
        //the for loop above takes the first 9 values the user inputs and stores them as matrix values.
        
        System.out.format("Enter %d vector values: ", VECTOR_SIZE);  
        //the for loop below allows for the first 3 values the user inputs to be stored in the vector array.
        for (int i = 0; i < VECTOR_SIZE; i++){
            double value = s.nextDouble();
            vector[i] = value;
        }
        s.close();
        //the declaration below calls the multiply method and will store the return value in an array called double.
        double [] result = multiply(matrix, vector);
        System.out.print("Result = [");
        printVector(result);
        System.out.println("]");
    }
    private static void printVector(double[] arr) {
        /*This for loop checks to see if the array element is the last one or not,
         if it is not then there is a space after the value prints */
        for (int i = 0; i < arr.length; i++){
            if (i == (arr.length - 1)){
                System.out.print(arr[i]);
            }
            else{
            System.out.print(arr[i]+ " ");
            }
        }
    }
    private static double[] multiply(double[] matrix, double[] vector) {
        //I decided to make a temporary array to hold the multiplied values in order for their sum to then be sent to the result array.
        double[] result = new double[VECTOR_SIZE];
        double[] temp = new double[MATRIX_SIZE];
    
        for (int row = 0; row < VECTOR_SIZE; row++) {
            for (int col = 0; col < VECTOR_SIZE; col++) {
                // Calculating the index of the current element in the matrix
                int index = row * VECTOR_SIZE + col;
                // Multiplying the corresponding element in the matrix and vector
                temp[index] = matrix[index] * vector[col];
            }
        }
    
        // Summation of the elements in each row of the temporary array so that we can get a clean VECTOR_SIZE array
        for (int i = 0; i < MATRIX_SIZE; i += VECTOR_SIZE) {
            double sum = 0;
            // Sum the elements in the current row
            for (int j = 0; j < VECTOR_SIZE; j++) {
                sum += temp[i + j];
            }
            // Storing the sum of the row in the result array
            result[i / VECTOR_SIZE] = sum;
        }
    
        return result;
    }
    
}