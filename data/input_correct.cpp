/*
 * Created Date: 2021-10-12 22:02:30
 * Author: yuyangz
 */

//input test file with all symbol defined.

int main() 
{
    int xVariable=1, y = 2;
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