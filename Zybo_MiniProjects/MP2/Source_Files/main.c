/*###############################################################################################################
#   Created by Ian O'Donnell
#   Date: 4/10/21
#################################################################################################################
#   Purpose: 
#	Synthesizable software application to implement simple image processing algorithm
#
#	Input: Image Array (Found in image.h)
#	Output: modified Image Array after it goes through an image processing algorithm. 
###############################################################################################################*/

//Includes
#include <stdio.h>
#include <math.h>
#include "image.h"

//Declare Global Variables that will be used to loop through the images
//Row / Col can be modified here to process an image of a different size. 
int row = 25;									
int col = 25;
int i, j;

/*###############################################################################################################
#	Purpose: Converts all pixels to either intenisty 0 or 255 based on the threshold value
#	Input: Threshold value (decimal from 0-1)
#	Global Variable Used: img matrix
#	Output: Binary Image matrix
###############################################################################################################*/
void I2B(float level){  
    int threshold = 256*level;                 //calculate threshold
    ROW: for(i = 0; i < row; i++){
        COL: for(j = 0; j < col; j++){
            if (img[i][j] < threshold){
                b[i][j] = 0;
            } else {
                b[i][j] = 1;
            }
        }
    }
}

/*###############################################################################################################
#	Purpose: Pads an image with all zeros around the border
#	Global Variable Used: img matrix
#	Output: Zero Padded matrix (Size 27 x 27)
###############################################################################################################*/
void ZeroPad(){
    //TODO - First zero pad the image
    int r = row + 2;  
    int c = col + 2;                             
    ROW: for(i = 0; i < r; i++) {      
        COLUMN: for(j = 0; j < c; j++){
            if (i == 0){
               zeropad[i][j] = 0;
            }
            else if (i == r-1){
                zeropad[i][j] = 0;
            }
            else if (j == 0){
                zeropad[i][j] = 0;
            }
            else if (j == c-1){
                zeropad[i][j] = 0;
            } else {
                zeropad[i][j] = img[i-1][j-1];   
            }
        }
    }
}

/*###############################################################################################################
#	Purpose: Assigns a new the value of each pixel based on the average values of the neighborhood 
#	Global Variable Used: zeropad image matrix
#	Output: Average Image Array
###############################################################################################################*/
void Avg(){
    int r = row + 2;  
    int c = col + 2;
    ROW: for(i = 0; i < r; i++) {      
        COLUMN: for(j = 0; j < c; j++){

                int m = i + 1;
                int n = j + 1;
                avg[i][j] = (zeropad[m-1][n] + zeropad[m+1][n] + zeropad[m][n] 
                            + zeropad[m-1][n-1] + zeropad[m+1][n-1] + zeropad[m][n-1]
                            + zeropad[m-1][n+1] + zeropad[m+1][n+1] + zeropad[m][n+1]) / 9;         
        }
    }
}
/*###############################################################################################################
#	Purpose: Inverts the pixel intensity values 
#	Global Variable Used: img matrix
#	Output:  Inverted image array
###############################################################################################################*/
void Inv(){
    ROW: for(i = 0; i < row; i++){
        COL: for(j = 0; j < col; j++){
            inv[i][j] = 255 - img[i][j]; 
        }
    }
}

/*###############################################################################################################
#	Purpose: Scales the each pixel in an array by a constant factor
#	Global Variable Used: img matrix
#	Input: scaling factor (Currently only scaled by int values)
#	Output:  Scaled image array
###############################################################################################################*/
void Scale(float w){
    ROW: for(i = 0; i < row; i++){
        COL: for(j = 0; j < col; j++){
            sca[i][j] = img[i][j] * w; 
        }
    }
} 

/*###############################################################################################################
#	Purpose: Compress Image using a simple compress algorithm
#	Global Variable Used: img matrix
#	Output:  compressed image array (625 x 2 ) = max length (value x count of occurances)
###############################################################################################################*/
int Compression(){
    int num = 1 ;
    int count = 0;
    ROW: for(i = 0; i < row; i++){
        COL: for(j = 0; j < col; j++){
            if (j == (col-1) && (img[i][j] == img[i+1][j-(col-1)])){    //if position is on last col of row - Check next row - first col
                num = num + 1;                                          // add one to num
            } 
            else if (img[i][j] ==  img[i][j+1]){                        //if position is anywhere else than last col, check next col. 
                num = num + 1;
            }
            else{                                                       //if the next item is not the same, store the value into an array
                comp_img[count][0] = img[i][j];                         //store the intensity value into position 0
                comp_img[count][1] = num;                               //store the count into position 1
                count = count + 1;                                      //increase array position 
                num = 1;                                                //reset num counter
                }
        }
    }
    return count;
}

/*###############################################################################################################
#	Purpose: Decompress the previous Compressed Image using a simple decompression algorithm
#	Global Variable Used: comp_img matrix, img matrix
#	Output:  decompressed matrix (25 x 25)
###############################################################################################################*/
void Decompression(int d_col){

//Loop through the decompression image 
    int im_row = 0;
    int im_col = 0;
    int value, count;
    for(int p = 0; p < d_col; p++){         		//Go through each pixel in decompressed image array
        value = comp_img[p][0];             		//store the value
        count = comp_img[p][1];             		//store the count of the values
       for (i = 0; i < count; i++){         		//Assign the Value - Count - Amount of times while updating the position. 
            decom_img[im_row][im_col] = value;    	//Assign Value 
        
            //Move to Next Position
            if (im_col % 24 == 0){          		//IF at end of row
                im_row = im_row + 1;        		//Move to next row
                im_col = im_col - 24;       		//move to col 0
            } else {                        		//if not at end of row
                im_col = im_col + 1;        		//update position to move to the next position
            }
       }
    }
}

/*###############################################################################################################
#	Purpose: Prints out all matrixs to the terminal
###############################################################################################################*/
void PrintOut(int count) //For Verifying Design is being manipulated (testing)
{
    printf("Image: \n");
    ROW1: for(i = 0; i < row; i++){
        COL1: for(j = 0; j < col; j++){
            printf("%d, ", img[i][j]);
        }
        printf("\n");
    }   

    printf("Binary \n");
    ROW2: for(i = 0; i < row; i++){
        COL2: for(j = 0; j < col; j++){
            printf("%d, ", b[i][j]);
        }
        printf("\n");
    }

    printf("Zero Pad \n");
    ROW3: for(i = 0; i < row + 2; i++){
        COL3: for(j = 0; j < col + 2; j++){
            printf("%d, ", zeropad[i][j]);
        }
        printf("\n");
    }

    printf("Average \n");
    ROW4: for(i = 0; i < row; i++){
        COL4: for(j = 0; j < col; j++){
            printf("%d, ", avg[i][j]);
        }
        printf("\n");
    }

    printf("Inversion \n");
    ROW5: for(i = 0; i < row; i++){
        COL5: for(j = 0; j < col; j++){
            printf("%d, ", inv[i][j]);
        }
        printf("\n");
    }

    printf("Scaling .5 \n");
    ROW6: for(i = 0; i < row; i++){
        COL6: for(j = 0; j < col; j++){
            printf("%d, ", sca[i][j]);
        }
        printf("\n");
    }
	
	printf("Compressed Image\n");
    for (int n = 0; n <= count; n++){
        printf("[%d, %d],", comp_img[n][0], comp_img[n][1]);
        if (n % 25 == 0){
            printf("\n");
        }
	}
	printf("This File got compressed to %d lines \n", count);
	
    printf("decompressed \n");
    ROW7: for(i = 0; i < row; i++){
        COL7: for(j = 0; j < col; j++){
            printf("%d, ", decom_img[i][j]);
        }
        printf("\n");
    }
}






/*###############################################################################################################
#	Main Function to call sub functions
###############################################################################################################*/
int main() 
{    
    I2B(.5);						//Convert to Binary
    ZeroPad();						//Zero Pad Image	
    Avg();							//Take Average of Image
    Inv();							//Invert Image
    Scale(.5);						//Scale Image
    int count = Compression();		//Compress Image
    Decompression(count);			//Decompress Image
    PrintOut(count);				//print all images to terminal
}