/*
 * Created Date: 2021-10-23 12:27:49
 * Author: yuyangz
 */

//input test file with some symbol undefined.

int main() 
{
    int xVariable^% =1, y = 2;
    int z = 0;
    if (xVariable < y){
        z = y/xVariable;
    } 
    else {
        z = xVariable*y;
    }
    while (z != 0) {
        z = z + xVariable;
    }

    return 0;
}

//End of file.