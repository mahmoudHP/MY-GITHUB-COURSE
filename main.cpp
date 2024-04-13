// FCAI – Structured Programming – 2024 - Assignment 3.
// File: CS112_A3_part1_28_20230573_20231175_20231154.cpp
// Purpose:This program is baby Photoshop and contains some filters that can modify images as you like
// Author1: Youssef Ahmed Fathi , ID1 : 20230573 .
// Author2: Manar Sameh Abdel Samad , ID2 : 20231175 .
// Author3: Mahmoud Alaa Mahmoud , ID3 : 20231154 .
// Emails: Youssefkeashta@gmail.com ,manarsameh274@gmail.com, mahmoudalaa0001234@gmail.com .
// ID1:20230573 _ he did  [Filter 1 ( GrayScale Conversion )]
// ID2:20231175 _ she did [Filter 3 (Invert Image)]
// ID3:20231154 _ he did  [Filter 2 (Black And White) , Filter 5 (Flip Image) , Filter 8 (Cropped Image)]


#include <iostream>
#include <string>
#include "vector"
#include "Image_Class.h"


using namespace std;


// Function to apply blur using 2D prefix sum
void applyBlur(Image& image, int blur_radius) {
    int rows = image.height;
    int cols = image.width;

    vector<vector<int>> blurred_image(rows, vector<int>(cols, 0));

    // Calculate prefix sum for the image
    vector<vector<int>> prefix_sum(rows + 1, vector<int>(cols + 1, 0));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            prefix_sum[i][j] = image(i - 1, j - 1, 0) + prefix_sum[i - 1][j] + prefix_sum[i][j - 1] - prefix_sum[i - 1][j - 1];
        }
    }

    // Apply blur effect
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x1 = max(0, i - blur_radius);
            int y1 = max(0, j - blur_radius);
            int x2 = min(rows - 1, i + blur_radius);
            int y2 = min(cols - 1, j + blur_radius);
            int area = (x2 - x1 + 1) * (y2 - y1 + 1);

            blurred_image[i][j] = (prefix_sum[x2 + 1][y2 + 1] - prefix_sum[x2 + 1][y1] - prefix_sum[x1][y2 + 1] + prefix_sum[x1][y1]) / area;
        }
    }

    // Copy the blurred image data back to the original image
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image(i, j, 0) = blurred_image[i][j];
            image(i, j, 1) = blurred_image[i][j];
            image(i, j, 2) = blurred_image[i][j];
        }
    }
    // Ask the user for the output image file name
    string output_filename;
    cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
    cin >> output_filename;

    // Save the inverted image
    image.saveImage(output_filename);
    system(output_filename.c_str());
}



// Function to Merge 2 images.
int merge_images (const string &image1,const string &image2) {
    Image my_image_1 (image1) ;
    Image my_image_2 (image2) ;
    int size_h = max(my_image_2.height,my_image_1.height) ;
    int size_w = max(my_image_1.width,my_image_2.width) ;
    // create an image to merge 2 images in.
    Image merged_Image (size_h,size_w) ;
    for (int i = 0; i < size_h; ++i) {
        for (int j = 0; j < size_w; ++j) {
            for (int k = 0; k < 3; ++k) {
                int pixel_value_1 = 0;
                int pixel_value_2 = 0;
                if (i < my_image_1.height && j < my_image_1.width) {
                    pixel_value_1 = my_image_1(i, j, k);
                }
                if (i < my_image_2.height && j < my_image_2.width) {
                    pixel_value_2 = my_image_2(i, j, k);
                }
                merged_Image(i, j, k) = (pixel_value_1 + pixel_value_2) / 2;
            }
        }
    }
    // Ask the user for the output image file name
    string output_filename = "merged_Image.png";
    // Save the inverted image
    merged_Image.saveImage(output_filename);
    system(output_filename.c_str());

    return 0;
}

// Function to invert colors of an image
int invert_image(const string& image_name) {
    // Open the image
    Image my_image(image_name);

    // Loop through each pixel of the image
    for (int i = 0; i < my_image.width; ++i) {
        for (int j = 0; j < my_image.height; ++j) {
            // For each pixel, loop through RGB channels
            for (int k = 0; k < 3; ++k) {
                // Invert the color value by subtracting it from 255
                my_image(i, j, k) = 255 - my_image(i, j, k);
            }
        }
    }

    // Ask the user for the output image file name
    string output_filename;
    cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
    cin >> output_filename;

    // Save the inverted image
    my_image.saveImage(output_filename);
    system(output_filename.c_str());
    return 0;
}

// ============================================================ //

// Function to convert an image to black and white
int black_and_white(const string& image_name) {
    // Open the image
    Image image(image_name);

    int threshold;
    // Ask the user to input the threshold value for black and white conversion
    cout << "Please enter the threshold value (0 to 255): ";
    cin >> threshold;

    // Ensure threshold value is within valid range (0 to 255)
    threshold = max(0, min(255, threshold));

    // Convert image to grayscale
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            // Convert each pixel to grayscale by taking the average of RGB values
            int gray = (image(i, j, 0) + image(i, j, 1) + image(i, j, 2)) / 3;
            // Set all RGB channels to the grayscale value
            image(i, j, 0) = gray;
            image(i, j, 1) = gray;
            image(i, j, 2) = gray;
        }
    }

    // Apply threshold to convert grayscale to black and white
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            // If grayscale value is below the threshold, set pixel to black
            if (image(i, j, 0) < threshold) {
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }
                // Otherwise, set pixel to white
            else {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
        }
    }

    // Ask the user for the output image file name
    string output_filename;
    cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
    cin >> output_filename;
    // Save the black and white image
    image.saveImage(output_filename);
    system(output_filename.c_str());
    return 0;
}

// ============================================================ //

// Function to flip an image horizontally or vertically
int flip_image(const string& image_name) {
    // Open the image
    Image image(image_name);

    char flipDirection;
    // Ask the user to choose flip direction (horizontal or vertical)
    cout << "Please choose flip direction ('H' for horizontal, 'V' for vertical): ";
    cin >> flipDirection;

    // Ensure valid flip direction
    flipDirection = toupper(flipDirection); // Convert to uppercase
    while (flipDirection != 'H' && flipDirection != 'V') {
        cout << "Invalid flip direction. Please enter 'H' for horizontal or 'V' for vertical: ";
        cin >> flipDirection;
        flipDirection = toupper(flipDirection);
    }

    // Flip the image based on user's choice
    if (flipDirection == 'H') { // Horizontal flip
        for (int i = 0; i < image.width / 2; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < 3; ++k) {
                    // Swap pixels between left and right sides of the image
                    swap(image(i, j, k), image(image.width - 1 - i, j, k));
                }
            }
        }
    }
    else { // Vertical flip
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height / 2; ++j) {
                for (int k = 0; k < 3; ++k) {
                    // Swap pixels between top and bottom sides of the image
                    swap(image(i, j, k), image(i, image.height - 1 - j, k));
                }
            }
        }
    }

    // Ask the user for the output image file name
    string output_filename;
    cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
    cin >> output_filename;
    // Save the flipped image
    image.saveImage(output_filename);
    system(output_filename.c_str());
    return 0;
}

// ============================================================ //

// Function to crop an image
int Cropping_Image(const string& image_name) {
    // Open the image
    Image My_Image(image_name);

    // Declare variables for width and height of cropping
    short w, h;
    cout << " Enter The Width And Height Of Cropping : ";
    cin >> w >> h;

    // Check if cropping area is within the image bounds
    if (w <= My_Image.width && h <= My_Image.height && w > 0 && h > 0) {
        // Create a new image for cropped result
        Image Cropped_Image(w, h);

        // Loop through the cropping area
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                // Copy pixel values from original image to cropped image
                for (int k = 0; k < 3; ++k) {
                    Cropped_Image(i, j, k) = My_Image(i, j, k);
                }
            }
        }

        // Ask the user for the output image file name
        string output_filename;
        cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
        cin >> output_filename;

        // Save the cropped image
        Cropped_Image.saveImage(output_filename);
        system(output_filename.c_str());
    }
    else {
        cout << "Invalid cropping dimensions. Please ensure width and height are within the image bounds and greater than zero.\n";
    }

    return 0;
}


// ============================================================ //
// Grayscale Conversion Filter

int Grayscale_Conversion (const string& File_Name ) {
    Image img (File_Name) ;
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < 3; ++k) {
                avg += img(i,j,k) ;
                avg / 3 ;
                img(i,j,0) = avg ;
                img(i,j,1) = avg ;
                img(i,j,2) = avg ;

            }

        }

    }
    // Ask the user for the output image file name
    string output_filename;
    cout << "Please enter the output image name with extension (.jpg, .bmp, .png, .tga): ";
    cin >> output_filename;

    // Save the cropped image
    img.saveImage(output_filename);
    system(output_filename.c_str());



    return 0;

}





int main() {
    char choice;
    string image_name;
    string image_name_2 ;
    while (true) {
        cout << "                              \t\t*/* Welcome to our program */*\n";
        cout << "this program is a baby Photo Shop that can apply some filters in your photo ";
        cout << "What would you like to do?\n";
        cout << "A: Apply a filter to an image.\n"
             << "B: Exit.\n";
        cin >> choice;
        if (choice == 'A') {
            cout << "Which filter would you like to apply?\n";
            cout << "1. Invert Image.\n";
            cout << "2. Black and White.\n";
            cout << "3. Flip Image.\n";
            cout << "4. Cropping_Image.\n";
            cout << "5. Grayscale Conversion.\n" ;
            cout << "6. Merge Images.\n";
            cout << "7. Blur Image.\n";
            char filter_choice;
            cin >> filter_choice;
            cout << "Please enter the image name: ";
            cin >> image_name;
            switch (filter_choice) {
                case '1':
                    invert_image(image_name);
                    break;
                case '2':
                    black_and_white(image_name);
                    break;
                case '3':
                    flip_image(image_name);
                    break;
                case '4':
                    Cropping_Image(image_name);
                    break;
                case '5':
                    Grayscale_Conversion(image_name);
                    break;
                case '6' :
                    cout << " Enter The Image 2 Name : ";
                    cin >> image_name_2 ;
                    merge_images(image_name,image_name_2) ;
                    break;
                case '7' : {
                    Image my_image(image_name);

                    // Blur radius
                    int blur_radius = 3;

                    // Apply blur to the image
                    applyBlur(my_image, blur_radius);

                    // Save the blurred image

                    applyBlur(my_image, blur_radius);
                    break;
                }
                default : {
                    cout << " Invalid Input.";
                    break;
                }
            }
        } else if (choice == 'B') {
            cout << "Thank you for using our filters.\n";
            break;
        } else {
            cout << "Please enter a valid input.\n";
            break;
        }
    }
    cout << "*/* Thank you */*\n";

    return 0;
}

